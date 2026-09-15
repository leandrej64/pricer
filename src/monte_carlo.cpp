#include "monte_carlo.hpp"

MonteCarloPricer::MonteCarloPricer(int nb_simulations,Options* p_option, BlackScholes* p_model):nb_simulations(nb_simulations),option(p_option),model(p_model){
} 

double MonteCarloPricer::compute_average(PnlVect * samples) {
    PnlVect* ones = pnl_vect_create_from_scalar(samples->size,1.0);
    double mean = pnl_vect_scalar_prod(ones,samples)/samples->size;
    return mean;
}

double MonteCarloPricer::compute_variance(PnlVect* samples,double mean){
    double variance = 0; 
    for(int i=0; i<samples->size; i++){ 
        variance+= pow((pnl_vect_get(samples,i) - mean),2);
    } 
    return variance/=samples->size;
}

PricingResults* MonteCarloPricer::price(PnlMat* market_data,double t,double fd_step,double interest_rate,int hedging_dates_number){

    //preparation 

    double regular_step_size = this->option->maturity/this->option->timestep_number;
    double market_step_size = this->option->maturity/hedging_dates_number; 
    int last_regular_index = int(t / regular_step_size);
    int last_known_index = std::min(int(t/market_step_size),market_data->m-1);
    double next_regular_time = (last_regular_index+1) * regular_step_size;
    double first_step_size = next_regular_time - t;
    PnlMat* past = pnl_mat_create(last_regular_index+1,market_data->n);
    PnlMat* path = pnl_mat_create(option->timestep_number+1,market_data->n);
    PnlVect* last_spots = pnl_vect_create(market_data->n);
    pnl_mat_get_row(last_spots,market_data,last_known_index);
    for(int i=0; i<last_regular_index+1;i+=1){
        for(int j=0;j<path->n;j++){
            double value = pnl_mat_get(market_data,i*regular_step_size/market_step_size,j);
            pnl_mat_set(path,i,j,value);
            pnl_mat_set(past,i,j,value);
        }
    }
    pnl_mat_set_row(past,last_spots,past->m-1);




    //simulation
    PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE); 
    pnl_rng_sseed(rng,2);

    PnlVect * payoffs = pnl_vect_create(this->nb_simulations);
    PnlMat * delta_matrix = pnl_mat_create(this->nb_simulations,path->n);

    for (int i=0; i<this->nb_simulations; i++){ 
        this->model->sample_path(past,regular_step_size,first_step_size,path,rng);
        double payoff = option->payoff(path);
        pnl_vect_set(payoffs,i,payoff);
        //deltas
        for(int j=0;j<path->n;j++){ 
            PnlMat*right_shift = pnl_mat_copy(path);
            PnlMat* left_shift = pnl_mat_copy(path);
            this->model->shift_asset(right_shift,path,j,fd_step);
            this->model->shift_asset(left_shift,path,j,-fd_step);
            double right_payoff =  option->payoff(right_shift);
            double left_payoff = option->payoff(left_shift); 
            pnl_mat_set(delta_matrix,i,j,right_payoff-left_payoff);
            pnl_mat_free(&right_shift);
            pnl_mat_free(&left_shift);
         }
    }


    //estimation 

    double price =  this->compute_average(payoffs)* std::exp(-interest_rate*option->maturity);
    double price_std = (std::sqrt(this->compute_variance(payoffs,price)) * std::exp(-interest_rate*option->maturity))/std::sqrt(this->nb_simulations);

    PnlVect* deltas = pnl_vect_create(path->n);
    PnlVect* deltas_std = pnl_vect_create(path->n); 
    PnlVect* shifted_payoffs = pnl_vect_create(this->nb_simulations);
    for(int j=0;j<path->n;j++){ 
        pnl_mat_get_col(shifted_payoffs,delta_matrix,j);
        double delta = this->compute_average(shifted_payoffs);
        double delta_std = this->compute_variance(shifted_payoffs,delta);

        double last_spot = pnl_mat_get(past,past->m-1,j);
        double coeff_delta = std::exp(-interest_rate*(option->maturity))/(2.0*last_spot*fd_step);
        double coeff_std = std::sqrt(coeff_delta)/std::sqrt(this->nb_simulations);
        pnl_vect_set(deltas,j,coeff_delta*delta);
        pnl_vect_set(deltas_std,j,coeff_std*delta_std);
    }
    

    pnl_mat_free(&path);
    return new PricingResults(price,price_std,deltas,deltas_std);
    
}

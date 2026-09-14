#include "monte_carlo.hpp"

MonteCarlo::MonteCarlo(int nb_simulations) : nb_simulations(nb_simulations){} 

double MonteCarlo::compute_average(PnlVect * samples) {
    PnlVect* ones = pnl_vect_create_from_scalar(samples->size,1);
    double mean = pnl_vect_scalar_prod(ones,samples)/(double)(samples->size);
    this->mean_estimate = mean;
    pnl_vect_free(&ones);
    return mean;
}

double MonteCarlo::compute_variance(PnlVect* samples) {
    for{int i=0, } 
    PnlVect * mean_vect = pnl_vect_create_from_scalar(samples->size,this->mean_estimate);
    pnl_vect_minus_vect(mean_vect,samples);
    double variance = pnl_vect_scalar_prod(mean_vect,mean_vect)/(double)(samples->size);
    this->variance_estimate = variance;
    pnl_vect_free(&mean_vect);
    return variance;
}

PricingResults* MonteCarlo::price(Options* option,PnlMat* past,int date_number,double t,BlackScholes bs_model,double fd_step,double interest_rate){
    int path_size = option->timestep_number + 1;
    int timestep = option->maturity/option->timestep_number * 365;
    PnlMat * path = pnl_mat_create(path_size,option->payoff_coeffs->size);
    PnlVect * spots = pnl_vect_create(option->payoff_coeffs->size);
    for(int i=0; i<date_number+1; i+=timestep){
        int path_index = i/timestep;
        pnl_mat_get_row(spots,past,i);
        pnl_mat_set_row(path,spots,path_index);
    }
    int shift = past->m%timestep;
    double first_yearly_time_step = shift/365;
    double yearly_time_step = option->maturity/option->timestep_number;
    PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE); 
    pnl_rng_sseed(rng,2);
    PnlVect * samples = pnl_vect_create(this->nb_simulations);
    PnlMat * delta_matrix = pnl_mat_create(this->nb_simulations,path->n);
    for (int i=0; i<this->nb_simulations; i++){ 
        bs_model.sample_path(past,t,path,rng);
        // pnl_mat_print(path);
        double payoff = option->payoff(path);
        // std::cout<< payoff << std::endl;
        pnl_vect_set(samples,i,payoff);
        for(int j=0;j<path->n;j++){ 
            PnlMat*right_shift = bs_model.shift_asset(path,j,fd_step);
            PnlMat*left_shift = bs_model.shift_asset(path,j,-fd_step);
            double right_payoff =  option->payoff(right_shift);
            double left_payoff = option->payoff(left_shift); 
            // std::cout << right_payoff - left_payoff<< std::endl;
            pnl_mat_set(delta_matrix,i,j,right_payoff-left_payoff);
            pnl_mat_free(&right_shift);
            pnl_mat_free(&left_shift);
         }
    }


    PnlVect* deltas = pnl_vect_create(path->n);
    PnlVect* deltas_std = pnl_vect_create(path->n); 
    PnlVect* shifted_payoffs = pnl_vect_create(this->nb_simulations);
    for(int j=0;j<path->n;j++){ 
        pnl_mat_get_col(shifted_payoffs,delta_matrix,j);
        double delta = this->compute_average(shifted_payoffs);
        double variance = this->compute_variance(shifted_payoffs);
        double last_spot = pnl_mat_get(past,past->m-1,j);
        double coeff_delta = std::exp(-interest_rate*(option->maturity))/(2.0*last_spot*fd_step);
        double coeff_std = std::sqrt(coeff_delta)/std::sqrt(this->nb_simulations);
        pnl_vect_set(deltas,j,coeff_delta*delta);
        pnl_vect_set(deltas_std,j,coeff_std*variance);
    }
    
    std::cout << this->compute_average(samples) << std::endl;
    double price = this->compute_average(samples) * std::exp(-interest_rate*option->maturity);
    double price_std = (std::sqrt(this->compute_variance(samples)) * std::exp(-interest_rate*option->maturity))/std::sqrt(this->nb_simulations);
    pnl_mat_free(&path);
    return new PricingResults(price,price_std,deltas,deltas_std);
    
}
#include "capi.hpp"
#include "black_scholes.hpp"
#include "json_parser.hpp"
#include "Options/asian_option.hpp"
#include "Options/basket_option.hpp"
#include "Options/perf_option.hpp"
#include "monte_carlo.hpp"
#include "pricing_results.hpp"

PricingResultC* price_option(const char* json_path, const char* market_path, double t){
    OptionData option_data = json_parser(json_path);
    PnlMat* market_data = pnl_mat_create_from_file(market_path);

    std::string option_type = option_data.option_type;
    Options* option = nullptr;
    if(option_type=="asian"){
        option = new AsianOption(option_data.maturity,option_data.strike,option_data.nb_dates,option_data.payoff_coeffs);
    }
    if(option_type=="basket"){
        option = new BasketOption(option_data.maturity,option_data.strike,option_data.nb_dates,option_data.payoff_coeffs);
    }
    if(option_type=="performance"){
        option = new PerfOption(option_data.maturity,option_data.strike,option_data.nb_dates,option_data.payoff_coeffs);
    }

    BlackScholes* black_scholes = new BlackScholes(option_data.interestRate,option_data.correlation,option_data.volatilities);
    MonteCarloPricer* monte_carlo = new MonteCarloPricer(50000,option,black_scholes);
    PricingResults* pricing_results = monte_carlo->price(market_data,t,option_data.fd_step,option_data.interestRate,option_data.hedging_dates_number);

    int nb_assets = option_data.nb_assets;
    PricingResultC* result = new PricingResultC();
    result->price = pricing_results->get_price();
    result->price_std_dev = pricing_results->get_price_std_dev();
    result->nb_assets = nb_assets;
    result->delta = new double[nb_assets];
    result->delta_std_dev = new double[nb_assets];
    const PnlVect* delta = pricing_results->get_delta();
    const PnlVect* delta_std_dev = pricing_results->get_delta_std_dev();
    for(int i=0;i<nb_assets;i++){
        result->delta[i] = pnl_vect_get(const_cast<PnlVect*>(delta),i);
        result->delta_std_dev[i] = pnl_vect_get(const_cast<PnlVect*>(delta_std_dev),i);
    }
    delete pricing_results;
    delete monte_carlo;
    delete black_scholes;
    delete option;
    pnl_mat_free(&market_data);
    return result;
    
}

void free_pricing_result(PricingResultC* result){
    if(result == nullptr) return;
    delete[] result->delta;
    delete[] result->delta_std_dev;
    delete result;
}

SpotsC* get_spots(const char* json_path, const char* market_path, double t){
    OptionData option_data = json_parser(json_path);
    PnlMat* market_data = pnl_mat_create_from_file(market_path);

    double market_step_size = option_data.maturity/option_data.hedging_dates_number;
    int last_known_index = std::min(int(t/market_step_size),market_data->m-1);

    SpotsC* result = new SpotsC();
    result->nb_assets = market_data->n;
    result->spots = new double[market_data->n];
    for(int j=0;j<market_data->n;j++){
        result->spots[j] = pnl_mat_get(market_data,last_known_index,j);
    }

    pnl_mat_free(&market_data);
    return result;
}

void free_spots(SpotsC* result){
    if(result == nullptr) return;
    delete[] result->spots;
    delete result;
}

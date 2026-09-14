#include "black_scholes.hpp"
#include <iostream>
#include "json_parser.hpp"
#include "asian_option.hpp"
#include "monte_carlo.hpp"
#include "asian_option.hpp"
#include "basket_option.hpp"
#include "perf_option.hpp"
#include <cmath>
#include <string>

int main(int argc, char **argv){ 
    OptionData option_data = json_parser(argv[1]);
    double step_size = (double)option_data.maturity/(double)option_data.nb_dates;
    BlackScholes bs = BlackScholes(option_data.interestRate,option_data.correlation,option_data.volatilities,step_size,option_data.hedging_dates_number+1);
    PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE); 
    pnl_rng_sseed(rng,2);
    PnlMat* past = pnl_mat_create(1,option_data.nb_assets);
    pnl_mat_set_row(past,option_data.spot,0);   
    std::string option_type = option_data.option_type;
    Options* option; 
    if(option_type=="asian"){ 
        option = new AsianOption(option_data.maturity,option_data.strike,option_data.nb_dates,option_data.payoff_coeffs);
    }

    if(option_type=="basket"){ 
        option = new BasketOption(option_data.maturity,option_data.strike,option_data.nb_dates,option_data.payoff_coeffs);
    }

     if(option_type=="performance"){ 
        option = new PerfOption(option_data.maturity,option_data.strike,option_data.nb_dates,option_data.payoff_coeffs);
    }
    
    double nb_simu = 50000;

    MonteCarlo monte_carlo = MonteCarlo(nb_simu); 
    PricingResults* pricing_results = monte_carlo.price(option,past,1,0.0,bs,option_data.fd_step,option_data.interestRate);
    std::cout << *pricing_results << std::endl;
    return 0;

}
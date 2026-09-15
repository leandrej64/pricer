#include "black_scholes.hpp"
#include <iostream>
#include "json_parser.hpp"
#include "Options/asian_option.hpp"
#include "monte_carlo.hpp"
#include "Options/asian_option.hpp"
#include "Options/basket_option.hpp"
#include "Options/perf_option.hpp"
#include <cmath>
#include <string>


int main(int argc, char** argv){
    OptionData option_data = json_parser(argv[1]);
    PnlMat* market_data = pnl_mat_create_from_file(argv[2]);
    double t = std::stod(argv[3]);

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


    BlackScholes* black_scholes = new BlackScholes(option_data.interestRate,option_data.correlation,option_data.volatilities);
    MonteCarloPricer* monte_carlo = new MonteCarloPricer(50000,option,black_scholes);
    PricingResults* pricing_results = monte_carlo->price(market_data,t,option_data.fd_step,option_data.interestRate,option_data.hedging_dates_number);
    std::cout << *pricing_results << std::endl; 
    return 0;
}
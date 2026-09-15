#pragma once
#include <pnl/pnl_matrix.h>
#include <pnl/pnl_vector.h>
#include "pricing_results.hpp"
#include "options.hpp"
#include <cmath>
#include "black_scholes.hpp"
#include "pricing_results.hpp"
#include <algorithm>

class MonteCarloPricer{ 
    public : 
    int nb_simulations;
    Options * option;
    BlackScholes * model;

    MonteCarloPricer(int nb_simulations,Options* option,BlackScholes* model);

    double compute_average(PnlVect* samples);
    double compute_variance(PnlVect* samples,double mean);
    PricingResults* price(PnlMat* market_data,double t,double fd_step,double interest_rate,int hedging_dates_number);

};




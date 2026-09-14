#pragma once
#include <pnl/pnl_matrix.h>
#include <pnl/pnl_vector.h>
#include "pricing_results.hpp"
#include "options.hpp"
#include <cmath>
#include "black_scholes.hpp"
#include "pricing_results.hpp"

class MonteCarlo{ 
    public : 
    int nb_simulations;
    double variance_estimate; 
    double mean_estimate; 

    MonteCarlo(int nb_simulations);

    PnlVect * sample();

    double compute_average(PnlVect* samples);
    double compute_variance(PnlVect* samples);
    PricingResults* price(Options* option,PnlMat* past,int date_number,double t,BlackScholes bs_model,double fd_step,double interest_rate);

};




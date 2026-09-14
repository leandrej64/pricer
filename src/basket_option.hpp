#pragma once
#include "options.hpp"

class BasketOption : public Options{ 
    public : 
    BasketOption(double p_maturity, double p_strike,int p_timestep_number, PnlVect* payoff_coeffs);
    double payoff(PnlMat* spots) override ;
};
#pragma once

#include "options.hpp"

class AsianOption : public Options { 
    public : 
    AsianOption(double p_maturity,double p_strike, int p_timestep_number, PnlVect* p_payoff_coeffs);
    double payoff(PnlMat* spots) override;

};

#pragma once
#include <pnl/pnl_matrix.h>
#include <algorithm>
#include <vector>
#include <numeric>


class Options {
    public:
        double maturity;
        double strike;
        int timestep_number;
        PnlVect* payoff_coeffs;

        Options(double p_maturity, double p_strike, int p_timestep_number,  PnlVect* p_payoff_coeffs=nullptr);
        virtual double payoff(PnlMat* path) = 0; 
};

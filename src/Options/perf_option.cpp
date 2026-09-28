#include "perf_option.hpp"



PerfOption::PerfOption(double p_maturity,double p_strike, int p_timestep_number,  PnlVect* p_payoff_coeffs):
Options(p_maturity,p_strike,p_timestep_number,p_payoff_coeffs){}

double PerfOption::payoff(PnlMat* spots){
    PnlVect * perf = pnl_mat_mult_vect(spots,this->payoff_coeffs);
    double payoff = 1;
    for(int i = 1; i<perf->size;i++){ 
        double ratio = pnl_vect_get(perf,i)/pnl_vect_get(perf,i-1);
        double partial_payoff = std::max(ratio-1.0,0.0);
        payoff += partial_payoff;
    }
    pnl_vect_free(&perf);
    return payoff;
};

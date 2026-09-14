#include "asian_option.hpp"



AsianOption::AsianOption(double p_maturity,double p_strike, int p_timestep_number,  PnlVect* p_payoff_coeffs):
Options(p_maturity,p_strike,p_timestep_number,p_payoff_coeffs){}

double AsianOption::payoff(PnlMat* spots){
    PnlVect* ones = pnl_vect_create_from_scalar(spots->m,1);  
    PnlVect * sum = pnl_mat_mult_vect_transpose(spots,ones);
    double value = pnl_vect_scalar_prod(this->payoff_coeffs,sum)/(double) spots->m;
    return std::max(value-this->strike,0.0);
};

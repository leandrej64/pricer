#include "basket_option.hpp"



BasketOption::BasketOption(double p_maturity,double p_strike, int p_timestep_number, PnlVect* p_payoff_coeffs):
Options(p_maturity,p_strike,p_timestep_number,p_payoff_coeffs){}


double BasketOption::payoff(PnlMat* spots){
    PnlVect * last_spots = pnl_vect_create(spots->n); 
    pnl_mat_get_row(last_spots,spots,spots->m-1);
    double value = pnl_vect_scalar_prod(last_spots,this->payoff_coeffs);
    return std::max(((double)(value-this->strike)),0.0);
};



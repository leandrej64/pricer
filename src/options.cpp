#include "options.hpp"


Options::Options(double p_maturity, double p_strike, int p_timestep_number, PnlVect* p_payoff_coeffs):
maturity(p_maturity), strike(p_strike), timestep_number(p_timestep_number), payoff_coeffs(p_payoff_coeffs){};


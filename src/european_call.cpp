#include "european_call.hpp"


EuropeanCall::EuropeanCall(double p_strike,double p_maturity,double p_spot): strike(p_strike),maturity(p_maturity),spot(p_spot){};

double EuropeanCall::payoff(double p_spot){
    return std::max(p_spot -this->strike,0); 
}





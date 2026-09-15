#include <pnl/pnl_matrix.h>
#include <algorithm>

class EuropeanCall{ 
    public :
    double strike; 
    double maturity;
    double spot;
    EuropeanCall(double p_strike,double p_maturity,double spot);
    double payoff(double spot); 
    
};


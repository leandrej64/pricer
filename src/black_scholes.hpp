#pragma once
#include <pnl/pnl_matrix.h>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_random.h>
#include <cmath>



class BlackScholes{
    public:
    double risk_free;
    double step_size;
    int path_length;
    PnlMat* correlation_matrix;
    PnlMat * choleski_matrix_transpose;
    PnlVect * volatilities;
    
    BlackScholes(double p_risk_free,PnlMat * p_correlation_matrix,PnlVect * p_volatilities,double p_step_size,int p_path_length);
    void sample_path(PnlMat* past,double t,PnlMat* path,PnlRng* rng);
    PnlMat* build_random_matrix(PnlRng* rng);
    PnlMat* shift_asset(PnlMat* path,int asset,double fd_step);

}; 
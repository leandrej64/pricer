#pragma once
#include <pnl/pnl_matrix.h>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_random.h>
#include <cmath>


class BlackScholes{
    public:
    double risk_free;
    PnlMat* correlation_matrix;
    PnlMat * choleski_matrix_transpose;
    PnlMat * choleski;
    PnlVect * volatilities;
    PnlMat * random_matrix;
    PnlMat * scalar_prod;
    PnlVect* random_vect;

    BlackScholes(double p_risk_free,PnlMat * p_correlation_matrix,PnlVect * p_volatilities);
    void sample_path(PnlMat* past,double regular_step_size, double first_step_size,PnlMat* path,PnlRng* rng);
    void shift_asset(PnlMat * shifted_path, PnlMat* path,int asset,double fd_step,int first_index);

}; 
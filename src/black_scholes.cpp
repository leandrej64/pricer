#include "black_scholes.hpp"
#include <fstream>
#include <iostream>

BlackScholes::BlackScholes(double p_risk_free,PnlMat * p_correlation_matrix,PnlVect * p_volatilities):risk_free(p_risk_free),correlation_matrix(p_correlation_matrix),volatilities(p_volatilities){
    PnlMat * choleski = pnl_mat_copy(p_correlation_matrix);
    int code = pnl_mat_chol(choleski);
    this->choleski = choleski;
    PnlMat * choleski_transpose = pnl_mat_copy(choleski);
    pnl_mat_sq_transpose(choleski_transpose);
    this->choleski_matrix_transpose = choleski_transpose;
    this-> random_vect = pnl_vect_create(choleski->n);

}

void BlackScholes::sample_path(PnlMat* past, double regular_step_size, double first_step_size, PnlMat* path,PnlRng* rng){ 

    PnlVect* choleski_row = pnl_vect_create_from_zero(past->n); 
    int last_index = past->m - 1;

    pnl_vect_rng_normal(this->random_vect,past->n,rng);
    for(int j=0;j<path->n;j++){ 
            pnl_mat_get_row(choleski_row,choleski,j);
            double sigma = pnl_vect_get(this->volatilities,j);
            double value = pnl_mat_get(past,last_index,j)*std::exp((this->risk_free - (double)(sigma * sigma)/2.0) * first_step_size +  sigma * std::sqrt(first_step_size) * pnl_vect_scalar_prod(this->random_vect,choleski_row));
            pnl_mat_set(path,last_index+1,j,value);
        }


    for(int i=last_index+2;i<path->m;i++){
        pnl_vect_rng_normal(this->random_vect,past->n,rng);
        for(int j=0;j<path->n;j++){ 
            pnl_mat_get_row(choleski_row,choleski,j);
            double sigma = pnl_vect_get(this->volatilities,j);
            double value = pnl_mat_get(path,i-1,j)*std::exp((this->risk_free - (double)(sigma * sigma)/2.0) * regular_step_size +  sigma * std::sqrt(regular_step_size) * pnl_vect_scalar_prod(this->random_vect,choleski_row));
            pnl_mat_set(path,i,j,value);
        }
    }

    pnl_vect_free(&choleski_row);
}

void BlackScholes::shift_asset(PnlMat * shifted_path, PnlMat* path,int asset,double fd_step){
    for(int i=0;i<path->m;i++){ 
        pnl_mat_set(shifted_path,i,asset,pnl_mat_get(path,i,asset)*(1+fd_step));
    }
}
#include "black_scholes.hpp"
#include <fstream>
#include <iostream>

BlackScholes::BlackScholes(double p_risk_free,PnlMat * p_correlation_matrix,PnlVect * p_volatilities,double p_step_size,int p_path_length):risk_free(p_risk_free),correlation_matrix(p_correlation_matrix),volatilities(p_volatilities),step_size(p_step_size),path_length(p_path_length){
    PnlMat * choleski = pnl_mat_copy(p_correlation_matrix);
    int code = pnl_mat_chol(choleski); 
    pnl_mat_sq_transpose(choleski);
    this->choleski_matrix_transpose = choleski; 

}


void BlackScholes::sample_path(PnlMat* past,double t,PnlMat* path,PnlRng* rng){ 
    PnlMat* random_matrix  = this->build_random_matrix(rng);
    // pnl_mat_print(random_matrix);
    for(int i=past->m;i<path->m;i++){
        for(int j=0;j<path->n;j++){ 
            double value = pnl_mat_get(path,i-1,j)*pnl_mat_get(random_matrix,i,j);
            // std::cout << value << std::endl;
            pnl_mat_set(path,i,j,value);
            // pnl_mat_print(path);
        }
    }
    pnl_mat_free(&random_matrix);

}

PnlMat * BlackScholes::build_random_matrix(PnlRng* rng){
    PnlMat * random_matrix = pnl_mat_create(this->path_length,this->volatilities->size);
    pnl_mat_rng_normal(random_matrix,random_matrix->m,random_matrix->n,rng);
    random_matrix  = pnl_mat_mult_mat(random_matrix,this->choleski_matrix_transpose);
    for(int i=0; i< random_matrix->m;i++){ 
        for(int j=0;j<random_matrix->n;j++){ 
            double sigma = pnl_vect_get(this->volatilities,j);
            double value = (this->risk_free - (double)(sigma * sigma)/2.0) * this->step_size +  sigma * std::sqrt(step_size) * pnl_mat_get(random_matrix,i,j); 
            pnl_mat_set(random_matrix,i,j,std::exp(value));
        }
    }    
    return random_matrix;

}

PnlMat* BlackScholes::shift_asset(PnlMat* path,int asset,double fd_step){
    PnlMat* shifted_path = pnl_mat_copy(path);
    PnlVect* asset_shifted = pnl_vect_create(path->m);
    pnl_mat_get_col(asset_shifted,path,asset); //deep copy
    pnl_vect_mult_scalar(asset_shifted,(double)(1+fd_step)); 
    pnl_mat_set_col(shifted_path,asset_shifted,asset);
    free(asset_shifted);
    return shifted_path;
}
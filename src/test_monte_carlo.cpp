#include "monte_carlo.hpp"
#include "pnl/pnl_random.h"
#include <iostream>

int main(){ 

    int nb_simu = 100;
    PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE); 
    pnl_rng_sseed(rng,2);
    PnlVect * random_vect = pnl_vect_create(nb_simu); 
    pnl_vect_rng_normal(random_vect,nb_simu,rng);
    MonteCarlo montecarlo = MonteCarlo(nb_simu);
    montecarlo.run(random_vect);
    std::cout << montecarlo.mean_estimate <<std::endl;
    std::cout << montecarlo.variance_estimate <<std::endl;
    return 0;

} 
#include "monte_carlo.hpp"

int main() {
    // 1. Création "classique" (alloué sur la Stack, comme une variable normale)
    
    MonteCarlo montecarlo("test");
    montecarlo.Display();
    return 0;
}
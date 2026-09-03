
#include <string>
#include "monte_carlo.hpp"
#include <iostream>

MonteCarlo::MonteCarlo(const std::string& Text)
    :text(Text)
    {}


void MonteCarlo::Display() const{
    std::cout << "Pièce : " << text << std::endl;
}
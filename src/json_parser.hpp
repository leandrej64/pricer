
#pragma once

#include <iostream>
#include <fstream>
#include "json_helper.hpp"


struct OptionData {
    int nb_assets;
    int nb_simulations;
    int nb_dates;
    double fd_step;
    double maturity;
    double interestRate;
    double strike;
    std::string option_type;
    PnlVect *volatilities;
    PnlVect *spot;
    PnlVect *payoff_coeffs;
    PnlMat *correlation;
    double hedging_dates_number;
};


OptionData json_parser(const char* json_file);
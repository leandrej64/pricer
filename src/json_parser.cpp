#include "json_parser.hpp"


OptionData json_parser(char* json_file){ 
    std::ifstream ifs(json_file);
    nlohmann::json j = nlohmann::json::parse(ifs);
    int nb_assets;
    int nb_simulations; 
    int nb_dates;
    double unique_correlation;
    double fd_step;
    double maturity;
    double strike; 
    double hedging_dates_number; 
    double interest_rate; 
    std::string option_type;
    PnlVect *volatilities;
    PnlVect *spot;
    PnlVect *payoff_coeffs;
    PnlMat *correlation;
  
    option_type = j.at("option type").get<std::string>();
    j.at("model size").get_to(nb_assets);
    j.at("volatility").get_to(volatilities);
    j.at("spot").get_to(spot);
    j.at("payoff coefficients").get_to(payoff_coeffs);
    j.at("correlation").get_to(unique_correlation);
    j.at("sample number").get_to(nb_simulations);
    j.at("timestep number").get_to(nb_dates);
    j.at("fd step").get_to(fd_step);
    j.at("maturity").get_to(maturity);
    j.at("interest rate").get_to(interest_rate);
    j.at("hedging dates number").get_to(hedging_dates_number);

    if (j.contains("strike")) {
        j.at("strike").get_to(strike);
    }
    else {
        strike = 0.0;
    }

    if (volatilities->size == 1 && nb_assets > 1) {
        pnl_vect_resize_from_scalar(volatilities, nb_assets, GET(volatilities, 0));
    }
    if (spot->size == 1 && nb_assets > 1) {
        pnl_vect_resize_from_scalar(spot, nb_assets, GET(spot, 0));
    }
    if (payoff_coeffs->size == 1 && nb_assets > 1) {
        pnl_vect_resize_from_scalar(payoff_coeffs, nb_assets, GET(payoff_coeffs, 0));
    }

    
    correlation = pnl_mat_create_from_scalar(nb_assets, nb_assets, unique_correlation);

    for (int i = 0; i < nb_assets; i++) {
        pnl_mat_set(correlation, i, i, 1.0);
    }

    return OptionData{
    .nb_assets = nb_assets,
    .nb_simulations = nb_simulations,
    .nb_dates = nb_dates,
    .fd_step = fd_step,
    .maturity = maturity,
    .interestRate = interest_rate,
    .strike = strike,
    .option_type = option_type,
    .volatilities = volatilities,
    .spot = spot,
    .payoff_coeffs = payoff_coeffs,
    .correlation = correlation,
    .hedging_dates_number = hedging_dates_number};
    
    }
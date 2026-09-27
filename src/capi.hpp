#pragma once

extern "C" {

struct PricingResultC {
    double price;
    double price_std_dev;
    int nb_assets;
    double* delta;
    double* delta_std_dev;
};

PricingResultC* price_option(const char* json_path, const char* market_path, double t);
void free_pricing_result(PricingResultC* result);

struct SpotsC {
    int nb_assets;
    double* spots;
};

SpotsC* get_spots(const char* json_path, const char* market_path, double t);
void free_spots(SpotsC* result);

}

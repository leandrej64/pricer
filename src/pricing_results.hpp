#pragma once
#include <iostream>
#include "pnl/pnl_vector.h"

class PricingResults
{
  private:
    double price;
    const PnlVect* delta;
    double priceStdDev;
    const PnlVect* deltaStdDev;

  public:
    PricingResults(double p_price, double p_priceStdDev, const PnlVect* const p_delta, const PnlVect* const p_deltaStdDev);

    double get_price() const;
    double get_price_std_dev() const;
    const PnlVect* get_delta() const;
    const PnlVect* get_delta_std_dev() const;

    friend std::ostream& operator<<(std::ostream& stm, const PricingResults& res);
};


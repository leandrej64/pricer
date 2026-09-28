# Pricer

Academic project (Ensimag) — a C++ derivatives pricer using naive Monte Carlo estimators, with underlying paths simulated under a multi-asset Black-Scholes model.

Course taught by Jérôme Lelong and Mnacho Echenim. Linear algebra computations rely on [PNL](https://github.com/pnlnum/pnl), an open-source C scientific library developed and maintained by Jérôme Lelong.

## Options priced

$\lambda \in \mathbb R^D$: payoff weights, $K$: strike, $`\lambda \cdot S_u = \sum_d \lambda_d S_u^d`$.

| Option | Payoff |
|---|---|
| Asian | $`\left(\frac{1}{N+1}\sum_{i=0}^{N} \lambda \cdot S_{t_i} - K\right)_+`$ |
| Basket | $`\left(\lambda \cdot S_T - K\right)_+`$ |
| Performance | $`1 + \sum_{i=1}^{N} \left(\frac{\lambda \cdot S_{t_i}}{\lambda \cdot S_{t_{i-1}}} - 1\right)_+`$ |

None of these has a closed-form price: each payoff depends on a sum of correlated lognormal variables (over dates and/or assets), whose law is not lognormal. Hence Monte Carlo.

## Overview

Given option parameters (`.json`) and market data (`.txt`), the pricer computes the option's price and deltas at any time before maturity.

| Class | Role |
|---|---|
| `Options` | Abstract base: maturity, strike, fixing dates, payoff weights; pure virtual `payoff(path)` |
| `AsianOption` / `BasketOption` / `PerfOption` | Concrete payoffs |
| `BlackScholes` | Correlated multi-asset model (Cholesky): `sample_path` simulates future spots, `shift_asset` bumps one asset for finite differences |
| `MonteCarloPricer` | Builds the known past from market data, runs the simulations, returns price and deltas with their std devs |
| `PricingResults` | Holds the price, deltas and std devs; JSON output |
| `OptionData` (struct) | Parameters parsed from the `.json` file |

## Method

### Notation

- $r$: risk-free rate. $T$: maturity. $t \in [0,T]$: pricing time. $M$: number of Monte Carlo paths.
- $t_0 < t_1 < \cdots < t_N = T$: the option's fixing (monitoring) dates, with $t_i \le t < t_{i+1}$.
- $\varphi$: the option's payoff function.
- $S_{t_0}, \ldots, S_t$: known past values of the underlying(s), read from market data — deterministic given $t$.
- $\tilde S^{(j)}_u$: $j$-th independent Monte Carlo draw of $\tilde S$, a process with the same law as $S$ but started at 1 and independent of $\mathcal F_t$, so that $`S_t\,\tilde S_u \overset{d}{=} S_{t+u} \mid \mathcal F_t`$ — this is what lets future spots be simulated from the known present value $S_t$.
- $\hat\sigma$: empirical standard deviation of the $M$ discounted simulated payoffs.
- $S_t^d$: current value (at time $t$) of asset $d$. $h$: finite-difference bump size (relative shift of the spot).
- Spots are simulated under the risk-neutral measure: drift $r$, no historical drift.

### Option pricing

Monte Carlo estimation via Black-Scholes path sampling:

```math
\hat V_t = e^{-r(T-t)}\,\frac{1}{M}\sum_{j=1}^{M} \varphi\!\left(S_{t_0}, \ldots, S_t,\; S_t\,\tilde S^{(j)}_{t_{i+1}-t}, \ldots, S_t\,\tilde S^{(j)}_{t_N-t}\right)
```

with 95% confidence interval $`\left[\hat V_t \pm 1.96\,\hat\sigma/\sqrt{M}\right]`$.

### Delta estimation

Estimated via finite differences on the shifted spot:

```math
\Delta_d \approx \frac{e^{-r(T-t)}}{2\,S_t^d\,h}\;\frac{1}{M}\sum_{j=1}^{M}\Big[\varphi\big(\ldots,\,S_t^d(1+h)\,\tilde S^{(j)},\ldots\big) - \varphi\big(\ldots,\,S_t^d(1-h)\,\tilde S^{(j)},\ldots\big)\Big]
```

## C API

A thin `extern "C"` bridge (`src/capi.hpp`/`src/capi.cpp`) exposes `price_option` and `get_spots` and is built as a shared library (`pricer_capi` CMake target, `libpricer_capi.so`), so the pricer can be consumed from other languages without linking against the C++/PNL toolchain directly. It's used this way by [delta-hedging](https://github.com/leandrej64/delta-hedging), a C# project backtesting delta-hedging strategies against this pricer via P/Invoke.

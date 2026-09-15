# Pricer

Academic project (Ensimag) — a C++ derivatives pricer using naive Monte Carlo estimators with a Black-Scholes model for sampling spots.

Course taught by Jérôme Lelong and Mnacho Echenim. Linear algebra computations rely on [PNL](<link>), an open-source C scientific library developed and maintained by Jérôme Lelong.

## Options priced

- Asian
- Basket
- Performance

## Overview

Given option parameters (`.json`) and market data (`.txt`), the pricer computes the option's price and deltas at any time before maturity.

## Method

### Notation

- $r$: risk-free rate. $T$: maturity. $t \in [0,T]$: pricing time. $M$: number of Monte Carlo paths.
- $t_0 < t_1 < \cdots < t_N = T$: the option's fixing (monitoring) dates, with $t_i \le t < t_{i+1}$.
- $\varphi$: the option's payoff function.
- $S_{t_0}, \ldots, S_t$: known past values of the underlying(s), read from market data — deterministic given $t$.
- $\tilde S^{(j)}_u$: $j$-th independent Monte Carlo draw of $\tilde S$, a process with the same law as $S$ but started at 1 and independent of $\mathcal F_t$, so that $S_t\,\tilde S_u \overset{d}{=} S_{t+u} \mid \mathcal F_t$ — this is what lets future spots be simulated from the known present value $S_t$.
- $\hat\sigma$: empirical standard deviation of the $M$ discounted simulated payoffs.
- $S_t^d$: current value (at time $t$) of asset $d$. $h$: finite-difference bump size (relative shift of the spot).

### Option pricing

Monte Carlo estimation via Black-Scholes path sampling:

$$
\hat V_t = e^{-r(T-t)}\,\frac{1}{M}\sum_{j=1}^{M} \varphi\!\left(S_{t_0}, \ldots, S_t,\; S_t\,\tilde S^{(j)}_{t_{i+1}-t}, \ldots, S_t\,\tilde S^{(j)}_{t_N-t}\right)
$$

with confidence interval $\left[\hat V_t \pm 1.96\,\hat\sigma/\sqrt{M}\right]$.

### Delta estimation (hedging)

Estimated via finite differences on the shifted spot:

$$
\Delta_d \approx \frac{e^{-r(T-t)}}{2\,S_t^d\,h}\;\frac{1}{M}\sum_{j=1}^{M}\Big[\varphi\big(\ldots,\,S_t^d(1+h)\,\tilde S^{(j)},\ldots\big) - \varphi\big(\ldots,\,S_t^d(1-h)\,\tilde S^{(j)},\ldots\big)\Big]
$$

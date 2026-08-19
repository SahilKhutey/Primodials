/**
 * Primordials — Lotka-Volterra Runge-Kutta 4th Order Dynamics (C++23)
 *
 * Mathematically stable differential equation integration.
 *
 * License: MIT
 */

#pragma once

#include <algorithm>
#include <cmath>
#include "Math.hpp"

namespace primordials::ecology {

struct LVParameters {
    float alpha = 1.4f;
    float beta  = 0.3f;
    float gamma = 0.01f;
    float delta = 0.1f;
    float epsilon = 0.4f;
    float carrying_capacity = 1000.0f;
};

struct LVState {
    float prey;
    float predators;
};

[[nodiscard]] inline LVState update_lv_rk4(
    LVState state,
    const LVParameters& params,
    float dt
) noexcept {
    auto deriv = [&](float x, float y) -> LVState {
        const float px = std::max(0.0f, x);
        const float py = std::max(0.0f, y);
        return LVState{
            params.alpha * px - params.beta * px * py - params.gamma * px * px,
            params.delta * px * py - params.epsilon * py
        };
    };

    const auto k1 = deriv(state.prey, state.predators);
    const auto k2 = deriv(state.prey + k1.prey * dt * 0.5f, state.predators + k1.predators * dt * 0.5f);
    const auto k3 = deriv(state.prey + k2.prey * dt * 0.5f, state.predators + k2.predators * dt * 0.5f);
    const auto k4 = deriv(state.prey + k3.prey * dt, state.predators + k3.predators * dt);

    LVState result;
    result.prey = state.prey + (k1.prey + 2.0f * k2.prey + 2.0f * k3.prey + k4.prey) * (dt / 6.0f);
    result.predators = state.predators + (k1.predators + 2.0f * k2.predators + 2.0f * k3.predators + k4.predators) * (dt / 6.0f);

    result.prey = math::clamp(result.prey, 0.0f, params.carrying_capacity);
    result.predators = math::clamp(result.predators, 0.0f, params.carrying_capacity);

    if (!std::isfinite(result.prey)) result.prey = 0.0f;
    if (!std::isfinite(result.predators)) result.predators = 0.0f;

    return result;
}

} // namespace primordials::ecology

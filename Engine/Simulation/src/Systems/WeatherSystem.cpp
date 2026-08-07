// Shape/Simulation/Systems/WeatherSystem.cpp
#include "Shape/Simulation/Systems/WeatherSystem.h"
#include <algorithm>

namespace Shape::Simulation {

WeatherSystem::WeatherSystem()
    : m_ticks_per_season(1200) {
    calculate_modifiers();
}

WeatherSystem::WeatherSystem(uint32_t ticks_per_season)
    : m_ticks_per_season(ticks_per_season > 0 ? ticks_per_season : 1200) {
    calculate_modifiers();
}

void WeatherSystem::update(const TickContext& ctx) {
    update_season(ctx.tick);
    update_weather_event(ctx);
    calculate_modifiers();
}

void WeatherSystem::force_season(Season s) {
    m_state.season = s;
    calculate_modifiers();
}

void WeatherSystem::force_weather(WeatherType w) {
    m_state.current_weather = w;
    calculate_modifiers();
}

void WeatherSystem::update_season(uint64_t current_tick) {
    uint32_t season_index = static_cast<uint32_t>((current_tick / m_ticks_per_season) % 4);
    switch (season_index) {
        case 0: m_state.season = Season::Spring; break;
        case 1: m_state.season = Season::Summer; break;
        case 2: m_state.season = Season::Autumn; break;
        case 3: m_state.season = Season::Winter; break;
    }
}

void WeatherSystem::update_weather_event(const TickContext& ctx) {
    if (m_weather_duration_ticks > 0) {
        m_weather_duration_ticks--;
        return;
    }

    // Roll for new weather event deterministically via ctx.rng
    uint32_t roll = ctx.rng.NextU32() % 100;
    m_weather_duration_ticks = 300 + (ctx.rng.NextU32() % 300); // 300..600 ticks

    switch (m_state.season) {
        case Season::Spring:
            if (roll < 50) m_state.current_weather = WeatherType::Clear;
            else if (roll < 85) m_state.current_weather = WeatherType::Rain;
            else m_state.current_weather = WeatherType::Storm;
            break;

        case Season::Summer:
            if (roll < 60) m_state.current_weather = WeatherType::Clear;
            else if (roll < 85) m_state.current_weather = WeatherType::Heatwave;
            else m_state.current_weather = WeatherType::Rain;
            break;

        case Season::Autumn:
            if (roll < 45) m_state.current_weather = WeatherType::Clear;
            else if (roll < 80) m_state.current_weather = WeatherType::Rain;
            else m_state.current_weather = WeatherType::Storm;
            break;

        case Season::Winter:
            if (roll < 40) m_state.current_weather = WeatherType::Clear;
            else if (roll < 80) m_state.current_weather = WeatherType::Blizzard;
            else m_state.current_weather = WeatherType::Rain;
            break;
    }

    // Update wind vector using deterministic RNG
    float wind_x = (static_cast<float>(ctx.rng.NextU32() % 200) - 100.0f) / 10.0f;
    float wind_y = (static_cast<float>(ctx.rng.NextU32() % 200) - 100.0f) / 10.0f;
    m_state.wind_vector = Math::Vector2f{wind_x, wind_y};
}

void WeatherSystem::calculate_modifiers() {
    float base_temp = 20.0f;
    float base_humidity = 0.5f;

    switch (m_state.season) {
        case Season::Spring: base_temp = 18.0f; base_humidity = 0.6f; break;
        case Season::Summer: base_temp = 32.0f; base_humidity = 0.4f; break;
        case Season::Autumn: base_temp = 12.0f; base_humidity = 0.65f; break;
        case Season::Winter: base_temp = -5.0f; base_humidity = 0.3f; break;
    }

    switch (m_state.current_weather) {
        case WeatherType::Clear:
            m_state.metabolic_drain_multiplier = 1.0f;
            m_state.resource_growth_multiplier = 1.0f;
            break;
        case WeatherType::Rain:
            base_humidity = std::min(1.0f, base_humidity + 0.3f);
            m_state.metabolic_drain_multiplier = 0.95f;
            m_state.resource_growth_multiplier = 1.3f;
            break;
        case WeatherType::Storm:
            base_humidity = 0.95f;
            m_state.metabolic_drain_multiplier = 1.2f;
            m_state.resource_growth_multiplier = 0.8f;
            break;
        case WeatherType::Blizzard:
            base_temp -= 15.0f;
            m_state.metabolic_drain_multiplier = 1.6f;
            m_state.resource_growth_multiplier = 0.1f;
            break;
        case WeatherType::Heatwave:
            base_temp += 12.0f;
            m_state.metabolic_drain_multiplier = 1.4f;
            m_state.resource_growth_multiplier = 0.6f;
            break;
    }

    m_state.temperature_celsius = base_temp;
    m_state.humidity = std::clamp(base_humidity, 0.0f, 1.0f);
}

} // namespace Shape::Simulation

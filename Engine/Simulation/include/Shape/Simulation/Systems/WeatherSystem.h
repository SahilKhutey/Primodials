// Shape/Simulation/Systems/WeatherSystem.h
#pragma once

#include "Simulation/ISimulationSystem.hpp"
#include "Math/Include/ShapeEngine/Math/Vector2.hpp"
#include <string_view>

namespace Shape::Simulation {

enum class Season {
    Spring,
    Summer,
    Autumn,
    Winter
};

enum class WeatherType {
    Clear,
    Rain,
    Storm,
    Blizzard,
    Heatwave
};

struct EnvironmentState {
    Season season = Season::Spring;
    WeatherType current_weather = WeatherType::Clear;
    float temperature_celsius = 20.0f;
    float humidity = 0.5f;             // 0.0 to 1.0
    Math::Vector2f wind_vector{0.0f, 0.0f};
    float metabolic_drain_multiplier = 1.0f;
    float resource_growth_multiplier = 1.0f;
};

class WeatherSystem : public ISimulationSystem {
public:
    WeatherSystem();
    explicit WeatherSystem(uint32_t ticks_per_season);
    virtual ~WeatherSystem() = default;

    void update(const TickContext& ctx) override;
    std::string_view name() const override { return "WeatherSystem"; }
    int priority() const override { return -40; } // Pre-Sim environment setup

    const EnvironmentState& state() const noexcept { return m_state; }
    Season current_season() const noexcept { return m_state.season; }
    WeatherType current_weather() const noexcept { return m_state.current_weather; }
    float temperature() const noexcept { return m_state.temperature_celsius; }

    void force_season(Season s);
    void force_weather(WeatherType w);

private:
    void update_season(uint64_t current_tick);
    void update_weather_event(const TickContext& ctx);
    void calculate_modifiers();

    uint32_t m_ticks_per_season = 1200; // 20 seconds at 60Hz per season
    EnvironmentState m_state;
    uint32_t m_weather_duration_ticks = 0;
};

} // namespace Shape::Simulation

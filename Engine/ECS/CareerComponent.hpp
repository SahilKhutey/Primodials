#pragma once

#include "Core/Platform.hpp"

namespace Shape {

enum class Department : u8 {
    Survival,
    Engineering,
    Science,
    Economy,
    Military,
    Exploration
};

enum class Profession : u8 {
    Citizen,
    Gatherer,
    Hunter,
    Builder,
    Soldier,
    Researcher,
    Trader,
    Governor,
    ChiefEngineer,
    SystemsOptimizer
};

struct CareerComponent {
    Department department = Department::Survival;
    Profession profession = Profession::Citizen;

    u32 rank = 1;
    u32 experience = 0;

    // Skill ratings (Practice increases these)
    f32 skillSurvival = 1.0f;
    f32 skillEngineering = 1.0f;
    f32 skillScience = 1.0f;
    f32 skillEconomy = 1.0f;
    f32 skillMilitary = 1.0f;
    f32 skillExploration = 1.0f;

    // Job preference interests (Determines role reassignment affinity)
    f32 interestSurvival = 1.0f;
    f32 interestEngineering = 1.0f;
    f32 interestScience = 1.0f;
    f32 interestEconomy = 1.0f;
    f32 interestMilitary = 1.0f;
    f32 interestExploration = 1.0f;

    f32 overallEfficiency = 1.0f;

    CareerComponent() = default;
};

} // namespace Shape

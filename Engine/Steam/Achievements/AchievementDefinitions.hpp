#pragma once

#include "Core/Platform.hpp"
#include <array>

namespace Shape::Steam {

enum class Achievement : u32 {
    FIRST_SPAWN = 0,
    FIRST_PREDATION = 1,
    FIRST_EXTINCTION = 2,
    HUNDREDTH_GENERATION = 3,
    THOUSANDTH_GENERATION = 4,
    IMMORTAL_LINEAGE = 5,
    STEADY_STATE = 6,
    MASS_EXTINCTION = 7,
    CARRYING_CAPACITY = 8,
    BOOM_AND_BUST = 9,
    POLYGON_PALETTE = 10,
    TRAIT_EXPLORER = 11,
    SPEED_DEMON = 12,
    APEX_PREDATOR = 13,
    FIRST_MOD = 14,
    MODDED_TIL_YOU_DROPPED = 15,
    SHIP_IT = 16,
    CURIOUS = 17,
    SCIENTIST = 18,
    RESEARCHER = 19,
    Count = 20
};

inline const char* g_achievementAPINames[] = {
    "ACH_FIRST_SPAWN", "ACH_FIRST_PREDATION", "ACH_FIRST_EXTINCTION", "ACH_HUNDREDTH_GEN",
    "ACH_THOUSANDTH_GEN", "ACH_IMMORTAL_LINEAGE", "ACH_STEADY_STATE", "ACH_MASS_EXTINCTION",
    "ACH_CARRYING_CAPACITY", "ACH_BOOM_AND_BUST", "ACH_POLYGON_PALETTE", "ACH_TRAIT_EXPLORER",
    "ACH_SPEED_DEMON", "ACH_APEX_PREDATOR", "ACH_FIRST_MOD", "ACH_MODDED_TIL_YOU_DROPPED",
    "ACH_SHIP_IT", "ACH_CURIOUS", "ACH_SCIENTIST", "ACH_RESEARCHER"
};

inline const char* g_achievementDisplayNames[] = {
    "First Breath", "Circle of Life", "Last One Standing", "Hundredth Generation",
    "Thousandth Generation", "Immortal Lineage", "Steady State", "Mass Extinction",
    "Carrying Capacity", "Boom and Bust", "Polygon Palette", "Trait Explorer",
    "Speed Demon", "Apex Predator", "Modder", "Modded Til You Dropped",
    "Ship It!", "Curious", "Scientist", "Researcher"
};

} // namespace Shape::Steam

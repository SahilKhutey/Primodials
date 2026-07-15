#include <catch2/catch_test_macros.hpp>
#include "Achievements.hpp"
#include <vector>
#include <cstdint>

using namespace PolygonalPrimordials;

TEST_CASE("Achievements Roundtrip — initialize and unlock", "[game][achievements]") {
    auto& a = Achievements::instance();
    a.initialize();
    REQUIRE(a.count() >= 25);

    // Reset before running
    a.reset_all();

    // Unlock some achievements
    REQUIRE(a.unlock("FIRST_LAUNCH"));
    REQUIRE(a.unlock("NEW_GAME"));
    REQUIRE_FALSE(a.add_progress("POPULATION_100", 50));
    REQUIRE(!a.get_progress("POPULATION_100")->unlocked);
    REQUIRE(a.add_progress("POPULATION_100", 60));  // Crosses threshold -> unlocks
    REQUIRE(a.get_progress("POPULATION_100")->unlocked);

    // Save
    std::vector<uint8_t> blob;
    a.save_state(blob);
    REQUIRE(blob.size() > 0);

    // Reset and reload
    a.reset_all();
    REQUIRE(a.get_progress("FIRST_LAUNCH")->unlocked == false);

    REQUIRE(a.load_state(blob));
    REQUIRE(a.get_progress("FIRST_LAUNCH")->unlocked == true);
    REQUIRE(a.get_progress("NEW_GAME")->unlocked == true);
    REQUIRE(a.get_progress("POPULATION_100")->unlocked == true);
    REQUIRE(a.get_progress("POPULATION_100")->current == 100);
    REQUIRE(a.get_progress("POPULATION_100")->target == 100);

    // Long IDs preserved
    REQUIRE(a.get_progress("POPULATION_10000") != nullptr);
    REQUIRE(a.get_progress("EVOLUTION_GOAL") != nullptr);

    // reset_all clears unlocks
    a.reset_all();
    REQUIRE(a.get_progress("FIRST_LAUNCH")->unlocked == false);
    REQUIRE(a.get_progress("FIRST_LAUNCH")->unlock_time_us == 0);
    REQUIRE(a.get_progress("POPULATION_100")->target == 100);  // target preserved
}

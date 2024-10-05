#include "framework/topological_sort.hxx"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("topological_sort sorts dependencies correctly", "[framework][unit]") {
    const auto dependencies = std::unordered_map<std::string, std::vector<std::string>>{
            {"WOOD", {"OAK", "PINE", "MAPLE"}},
            {"OAK", {"WHITE_OAK"}},
            {"PINE", {"WESTERN_PINE", "EASTERN_PINE"}},
            {"MAPLE", {"SUGAR_MAPLE", "RED_MAPLE"}},
        };
    const auto result = topological_sort(dependencies);

    // ensures parent appears before child while ignoring sibling order
    auto check = [&](const std::string& parent, const std::vector<std::string>& children) {
        auto parent_pos = std::find(result.sorted_entities.begin(), result.sorted_entities.end(), parent);
        for (const auto& child : children) {
            auto child_pos = std::find(result.sorted_entities.begin(), result.sorted_entities.end(), child);
            REQUIRE(parent_pos < child_pos);
        }
    };

    REQUIRE(result.status == TopologicalSortStatus::Ok);

    check("WOOD", {"OAK", "PINE", "MAPLE"});
    check("OAK", {"WHITE_OAK"});
    check("PINE", {"WESTERN_PINE", "EASTERN_PINE"});
    check("MAPLE", {"SUGAR_MAPLE", "RED_MAPLE"});
}

TEST_CASE("topological_sort detects cycles", "[framework][unit]") {
    auto dependencies = std::unordered_map<std::string, std::vector<std::string>>{
        {"WOOD", {"OAK"}},
        {"OAK", {"WHITE_OAK"}},
        {"WHITE_OAK", {"WOOD"}},
    };
    const auto result = topological_sort(dependencies);

    REQUIRE(result.status == TopologicalSortStatus::CycleDetected);
    REQUIRE(result.cycle_entities == std::vector<std::string>{ "WHITE_OAK", "OAK", "WOOD", "WHITE_OAK" });
}

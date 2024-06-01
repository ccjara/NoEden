#include <catch2/catch.hpp>
#include "lang/translator.hxx"

TEST_CASE("Translator::translate and operator() look up translation in dictionary", "[lang][unit]") {
    Translator t(Dictionary({
        {"WEAPON_AXE",    "axe"},
        {"WEAPON_SWORD",  "sword"},
        {"WEAPON_DAGGER", "dagger"}
    }));

    REQUIRE(t.translate("WEAPON_AXE") == "axe");
    REQUIRE(t.translate("WEAPON_SWORD") == "sword");
    REQUIRE(t.translate("WEAPON_DAGGER") == "dagger");

    REQUIRE(t("WEAPON_AXE") == "axe");
    REQUIRE(t("WEAPON_SWORD") == "sword");
    REQUIRE(t("WEAPON_DAGGER") == "dagger");
}

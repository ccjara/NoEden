#include <catch2/catch.hpp>
#include "lang/translation_loader.hxx"

TEST_CASE("load(std::string_view) loads the toml file at the given path", "[lang][unit]") {
    const auto tempFilePath = std::filesystem::temp_directory_path() / "translation_loader_test_file.txt";
    std::ofstream tempFile(tempFilePath);
    tempFile << "WEAPON_AXE=\"axe\"\n";
    tempFile << "WEAPON_SWORD=\"sword\"\n";
    tempFile << "WEAPON_DAGGER=\"dagger\"\n";
    tempFile.close();

    TranslationLoader loader;

    auto result = loader.load(tempFilePath.string());
    std::filesystem::remove(tempFilePath);

    REQUIRE(result.error == LoadTranslationError::None);
    auto tAxe = result.dictionary.get("WEAPON_AXE");
    REQUIRE(tAxe.has_value());
    REQUIRE(tAxe.value().get() == "axe");

    auto tSword = result.dictionary.get("WEAPON_SWORD");
    REQUIRE(tSword.has_value());
    REQUIRE(tSword.value().get() == "sword");

    auto tDagger = result.dictionary.get("WEAPON_DAGGER");
    REQUIRE(tDagger.has_value());
    REQUIRE(tDagger.value().get() == "dagger");
}

TEST_CASE("load(std::string_view) returns FileCorrupted when file not found", "[lang][unit]") {
    const auto tempFilePath = std::filesystem::temp_directory_path() / "translation_file_does_not_exist.toml";

    TranslationLoader loader;
    auto result = loader.load(tempFilePath.string());

    REQUIRE(result.error == LoadTranslationError::FileCorrupted);
}

TEST_CASE("load(std::string_view) returns FileCorrupted when file is not toml", "[lang][unit]") {
    const auto tempFilePath = std::filesystem::temp_directory_path() / "translation_loader_corrupted_translation.txt";
    std::ofstream tempFile(tempFilePath);
    tempFile << "Nice weather today, isn't it?\n";
    tempFile.close();

    TranslationLoader loader;
    auto result = loader.load(tempFilePath.string());

    REQUIRE(result.error == LoadTranslationError::FileCorrupted);
}

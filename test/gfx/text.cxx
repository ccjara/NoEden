#include "gfx/text.hxx"
#include <catch2/catch_test_macros.hpp>

std::string to_str(const TextOp& op) {
    std::string s(op.glyphs.size(), '\0');
    std::ranges::transform(op.glyphs, s.begin(), [](const i32 glyph) {
        return static_cast<char>(glyph);
    });
    return s;
}

TEST_CASE("Text parsing", "[gfx][unit]") {
    Text text;
    text.set_region({ 5000, 500 });

    SECTION("Handles empty text") {
        text.set_text("");
        text.update();

        REQUIRE(text.ops().empty());
    }

    SECTION("Works with simple text") {
        text.set_text("Hello World");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 1);
        REQUIRE(to_str(ops[0]) == "Hello World");
    }

    SECTION("Ignores command only string") {
        text.set_text("$");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.empty());
    }

    SECTION("Correctly handles escaped dollar sign") {
        text.set_text("$$");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 1);
        REQUIRE(to_str(ops[0]) == "$");
    }

    SECTION("Handles three dollar signs") {
        text.set_text("$$$");
        text.update();

        auto& ops = text.ops();

        REQUIRE(ops.size() == 1);
        REQUIRE(to_str(ops[0]) == "$");
    }

    SECTION("Ignores command char at end of string") {
        text.set_text("abc$");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 1);
        REQUIRE(to_str(ops[0]) == "abc");
    }

    SECTION("Outputs escaped dollar at end of string") {
        text.set_text("abc$$");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 1);
        REQUIRE(to_str(ops[0]) == "abc$");
    }

    SECTION("Ignores command char on unknown command specifier") {
        text.set_text("a$b");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 1);
        REQUIRE(to_str(ops[0]) == "ab");
    }

    SECTION("Removes newline command char") {
        text.set_text("a$nb");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 2);
        REQUIRE(to_str(ops[0]) == "a");
        REQUIRE(to_str(ops[1]) == "b");
    }

    SECTION("Removes color command string") {
        text.set_text("a$cFF0000b");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 2);
        REQUIRE(to_str(ops[0]) == "a");
        REQUIRE(to_str(ops[1]) == "b");
    }

    SECTION("Removes truncated color command at the end of the text") {
        const auto strings = {
            "a$cFF000",
            "a$cFF00",
            "a$cFF0",
            "a$cFF",
            "a$cF",
            "a$c",
        };

        for (auto& str : strings) {
            text.set_text(str);
            text.update();

            const auto& ops = text.ops();

            REQUIRE(ops.size() == 1);
            REQUIRE(to_str(ops[0]) == "a");
        }
    }

    SECTION("Removes trailing color command at the end of the text") {
        text.set_text("a$cAABBCC");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 1);
        REQUIRE(to_str(ops[0]) == "a");
    }

    SECTION("Removes color command and restoration substring") {
        text.set_text("a$cFF0000b$rc");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 3);
        REQUIRE(to_str(ops[0]) == "a");
        REQUIRE(to_str(ops[1]) == "b");
        REQUIRE(to_str(ops[2]) == "c");
    }

    SECTION("Restores previous color") {
        text.set_text("White$cFF0000Red$c00FF00Green$rRed$rWhite");
        text.update();

        const auto& ops = text.ops();

        REQUIRE(ops.size() == 5);
        REQUIRE(to_str(ops[0]) == "White");
        REQUIRE(ops[0].color == Color(0xFFFFFF));
        REQUIRE(to_str(ops[1]) == "Red");
        REQUIRE(ops[1].color == Color(0xFF0000));
        REQUIRE(to_str(ops[2]) == "Green");
        REQUIRE(ops[2].color == Color(0x00FF00));
        REQUIRE(to_str(ops[3]) == "Red");
        REQUIRE(ops[3].color == Color(0xFF0000));
        REQUIRE(to_str(ops[4]) == "White");
        REQUIRE(ops[4].color == Color(0xFFFFFF));
    }

    SECTION("Glyph command") {
        SECTION("Handles glyphs inside of ASCII range") {
            text.set_text("AB$g0043");
            text.update();

            const auto& ops = text.ops();

            REQUIRE(ops.size() == 1);
            REQUIRE(to_str(ops[0]) == "ABC");
        }

        SECTION("Handles glyphs outside of ASCII range") {
            text.set_text("A$g019bB");
            text.update();

            const auto& ops = text.ops();

            REQUIRE(ops.size() == 1);
            const auto& op = ops[0];
            REQUIRE(op.glyphs.size() == 3);
            REQUIRE(op.glyphs[0] == 'A');
            REQUIRE(op.glyphs[1] == 0x19b);
            REQUIRE(op.glyphs[2] == 'B');
        }

        SECTION("Supports mixed case") {
            text.set_text("A$gdEaDB");
            text.update();

            const auto& ops = text.ops();

            REQUIRE(ops.size() == 1);
            const auto& op = ops[0];
            REQUIRE(op.glyphs.size() == 3);
            REQUIRE(op.glyphs[0] == 'A');
            REQUIRE(op.glyphs[1] == 0xDEAD);
            REQUIRE(op.glyphs[2] == 'B');
        }

        SECTION("Ignores corrupted glyph codes") {
            text.set_text("A$gDEZDB");
            text.update();

            const auto& ops = text.ops();

            REQUIRE(ops.size() == 1);
            REQUIRE(to_str(ops[0]) == "AB");
        }

        SECTION("Ignores truncated glyph codes") {
            const auto strings = {
                "a$g555",
                "a$g55",
                "a$g5",
                "a$g",
            };

            for (const auto& str : strings) {
                text.set_text(str);
                text.update();

                const auto& ops = text.ops();

                REQUIRE(ops.size() == 1);
                REQUIRE(to_str(ops[0]) == "a");
            }
        }

        SECTION("Eliminates follow up characters when within glyph command range") {
            text.set_text("H$gello World");
            text.update();

            const auto& ops = text.ops();

            REQUIRE(ops.size() == 1);
            REQUIRE(to_str(ops[0]) == "H World");
        }
    }
}

#ifndef JARALYN_ARCHETYPE_HXX
#define JARALYN_ARCHETYPE_HXX

/**
 * @brief Actor templates used to construct Actors of one kind.
 * 
 * Every Archetype is statically instantiated once and may later be generated
 * from scripts (TODO). Actors all point to this singleton so their kind can
 * be determined at runtime.
 */
struct Archetype {
    /**
     * @brief Display drawing instructions
     */
    struct {
        /**
         * @brief The glyph to render for all Actors of this Archetype.
         */
        u32 glyph { 0 };

        /**
         * @brief The color to use when rendering the glyph.
         */
        Color color;

        /**
         * @brief Skips drawing this glyph if false.
         */
        bool visible { true };
    } display_info;

    /**
     * @brief Name of this archetype, such as "Troll", "Dwarf", etc.
     */
    std::string name;

    /**
     * @brief Base speed every Actor of this kind inherits.
     */
    i32 speed;
};

// TODO: should later be creatable in scripts

namespace {
    static Archetype troll() {
        Archetype arch;
        arch.name = "Troll";
        arch.speed = 75;
        arch.display_info.color = Color::red();
        arch.display_info.glyph = 'T';
        return arch;
    }

    static Archetype dwarf() {
        Archetype arch;
        arch.name = "Dwarf";
        arch.speed = 100;
        arch.display_info.color = Color::white();
        arch.display_info.glyph = '@';
        return arch;
    }
}

namespace Archetypes {
    const static Archetype Troll { troll() };
    const static Archetype Dwarf { dwarf() };
}

#endif

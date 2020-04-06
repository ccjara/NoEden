#ifndef JARALYN_SYSTEM_DEFINITION_HXX
#define JARALYN_SYSTEM_DEFINITION_HXX

#include "../engine/framework/game_system.hxx"

namespace system_id {
    constexpr system_id_t invalid { -1 };
    constexpr system_id_t gfx { 0 };
    constexpr system_id_t input { 1 };
    constexpr system_id_t unit { 2 };
    constexpr system_id_t grid { 3 };
}

#endif

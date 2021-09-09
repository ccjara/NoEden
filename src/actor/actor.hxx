#ifndef JARALYN_ACTOR_HXX
#define JARALYN_ACTOR_HXX

#include "archetype.hxx"
#include "../ai/ai_node.hxx"

struct Actor {
    const u64 id;
    static u64 next_id_;

    const Archetype* archetype;

    Vec2<i32> position;

    i32 speed { 0 };
    i32 energy { 0 };

    // FIXME: laying out our data like this kills the cache lol
    AiPrioritySelector ai;

    explicit Actor(const Archetype* arch);
};

#endif

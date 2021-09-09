#include "actor.hxx"

Actor::Actor(const Archetype* arch) : id { next_id_++ } {
    assert(arch);
    archetype = arch;
    speed = arch->speed;
}

u64 Actor::next_id_ { 0 };

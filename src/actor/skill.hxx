#ifndef JARALYN_SKILL_HXX
#define JARALYN_SKILL_HXX

enum class SkillId {
    Swim,
};

struct Skill {
    SkillId id;
    i32 level = 0;
    i32 progress = 0;
};

#endif

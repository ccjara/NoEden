#ifndef JARALYN_SKILL_HXX
#define JARALYN_SKILL_HXX

enum class SkillId {
    Swim,
};

const std::unordered_map<SkillId, const char*> skill_labels = {
    { SkillId::Swim, "SKILL_SWIMMING" }
};

struct Skill {
    SkillId id;
    i32 level = 0;
    i32 progress = 0;

    const char* label() {
        const auto& iter = skill_labels.find(id);
        if (iter == skill_labels.end()) {
            return "SKILL_UNKNOWN";
        }
        return iter->second;
    }
};

#endif

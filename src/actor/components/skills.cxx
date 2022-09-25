#include "skills.hxx"

Skills::Skills() : Component(ComponentType::Skills) {
}

const Skills::SkillContainer& Skills::skills() const {
    return skills_;
}

void Skills::increase_progress(SkillId id, i32 amount) {
    skills_[id].progress += amount;
}

void Skills::set_progress(SkillId id, i32 amount) {
    skills_[id].progress = amount;
}

std::unique_ptr<Component> Skills::clone() const {
    return std::make_unique<Skills>(*this);
}

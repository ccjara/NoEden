#include "attribute_bearing.hxx"

jc_attribute_bearing::jc_attribute_bearing() {
    attributes_[j_attribute_type::strength] = j_attribute {
        "Strength",
        j_attribute_type::strength,
    };
    attributes_[j_attribute_type::agility] = j_attribute {
        "Agility",
        j_attribute_type::agility,
    };
    attributes_[j_attribute_type::stamina] = j_attribute {
        "Stamina",
        j_attribute_type::stamina,
    };
}

void jc_attribute_bearing::modify(j_attribute_type type, int32_t value) {
    auto& attr { attributes_.at(type) };

    attr.points += value;
}

const j_attribute& jc_attribute_bearing::attribute(j_attribute_type type) {
    return attributes_.at(type);
}

const std::unordered_map<j_attribute_type, j_attribute>& jc_attribute_bearing::attributes() const {
    return attributes_;
}

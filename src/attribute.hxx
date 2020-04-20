#ifndef JARALYN_SKILL_HXX
#define JARALYN_SKILL_HXX

enum j_attribute_type {
	invalid,
	strength,
};

enum j_attribute_threshold {
	nonexistent = 0,
	poor = 10,
	very_low = 50,
	low = 75,
	average = 100,
	high = 150,
	very_high = 250,
	inconceivable  = 500
};

static const std::unordered_map<j_attribute_type, std::string_view> attribute_name {
	{ j_attribute_type::invalid, "Invalid" },
	{ j_attribute_type::strength, "Strength" },
};

struct j_attribute {
	j_attribute_type type { j_attribute_type::invalid };
	int32_t points { 0 };
};

#endif

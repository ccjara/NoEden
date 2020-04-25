#ifndef JARALYN_ATTRIBUTE_HXX
#define JARALYN_ATTRIBUTE_HXX

enum j_attribute_type {
	strength,
	agility,
	stamina,
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

struct j_attribute {
	std::string_view name;
	j_attribute_type type;
	int32_t points { 0 };
};

#endif

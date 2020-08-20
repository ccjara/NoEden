#ifndef JARALYN_ITEM_HXX
#define JARALYN_ITEM_HXX

enum class j_item_quality {
    flawed,
    common,
    sublime,
    exceptional,
    marvellous,
    supreme
};

class j_item : public j_identity<j_item> {
public:
    std::string label;
    int32_t durability { 0 };
    j_item_quality quality { j_item_quality::common };
};

#endif

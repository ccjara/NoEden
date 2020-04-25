#ifndef JARALYN_INVENTORY_HXX
#define JARALYN_INVENTORY_HXX

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

class jc_item_container {
protected:
    std::unordered_map<j_id, j_item> items_;
public:
    void put(j_item&& item);

    const std::unordered_map<j_id, j_item>& items() const noexcept;
};

#endif

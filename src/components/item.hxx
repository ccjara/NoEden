#ifndef JARALYN_ITEM_HXX
#define JARALYN_ITEM_HXX

class j_item : public j_identity<j_item> {
public:
    std::string label;
    int32_t durability { 0 };
};

#endif

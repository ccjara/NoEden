#ifndef JARALYN_ITEM_HXX
#define JARALYN_ITEM_HXX

class jc_item {
};

class j_item_container {
private:
    std::vector<entt::entity> items_;
public:
    const std::vector<entt::entity>& items() const;

    void store(entt::entity item);
};

class jc_inventory {
private:
    std::vector<j_item_container> containers_;
public:
    void put(uint32_t container_index, entt::entity item);

    j_item_container* create_container();
    j_item_container* get_container(uint32_t index);
};

#endif

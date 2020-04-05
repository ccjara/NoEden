#ifndef JARALYN_RESOURCE_MANAGER_HXX
#define JARALYN_RESOURCE_MANAGER_HXX

class resource_manager {
private:
    friend class manager_provider;

    void startup();
    void shutdown() noexcept;
public:
    std::ifstream load_file(const std::string& path) const;
};

#endif
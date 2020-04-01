#ifndef JARALYN_RESOURCE_MANAGER_HXX
#define JARALYN_RESOURCE_MANAGER_HXX

#include "../manager.hxx"

class resource_manager : public manager<resource_manager> {
private:
    friend class engine;
    void startup();
    void shutdown() noexcept;
public:
    std::ifstream load_file(const std::string& path) const;
};

#endif
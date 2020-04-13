#ifndef JARALYN_RESOURCE_MANAGER_HXX
#define JARALYN_RESOURCE_MANAGER_HXX

#include "resource_loader.hxx"

class j_resource_manager : public j_resource_loader {
public:
    std::ifstream load_file(const std::string& path) const override;

    void startup();
    void shutdown();
};

#endif
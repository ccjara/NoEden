#ifndef JARALYN_RESOURCE_LOADER_HXX
#define JARALYN_RESOURCE_LOADER_HXX

class j_resource_loader {
public:
    virtual std::ifstream load_file(const std::string& path) const = 0;
    virtual ~j_resource_loader() = default;
};

#endif

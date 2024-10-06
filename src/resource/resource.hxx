#pragma once

#include "resource/resource_type.hxx"

class Resource {
public:
    Resource(ResourceType type, std::string_view id, std::string&& path) : id_(id), path_(std::move(path)), type_(type) {}
    Resource(ResourceType type, std::string_view id, std::string_view path) : id_(id), path_(path), type_(type) {}

    [[nodiscard]] std::string_view id() const {
        return id_;
    }

    [[nodiscard]] std::string_view path() const {
        return path_;
    }

    [[nodiscard]] ResourceType type() const {
        return type_;
    }

    virtual ~Resource() = default;
protected:
    std::string id_;
    std::string path_;
    ResourceType type_;
};

template<typename T>
concept ResourceLike = std::is_base_of_v<Resource, T>;

#pragma once

struct ThermalProperties {
    /**
     * @brief Ignition temperature in Celsius
     */
    i32 ignites_at = 0;
};

struct Material {
    /**
     * @brief Unique identifier of this material
     */
    std::string id = {};

    /**
     * @brief Catalog source file name
     */
    std::string source = {};

    /**
    * @brief Parent material id
    */
    std::string extends = {};

    /**
     * @brief Pointer to parent material
     */
    const Material* parent = nullptr;

    /**
     * @brief Category tags
     */
    std::unordered_set<std::string> categories = {};

    /**
     * @brief Thermal properties
     */
    ThermalProperties thermal_properties = {};

    /**
     * @brief Copies inheritable properties from parent material into this material
     */
    void inherit_from(const Material& parent) {
        categories = parent.categories;
        thermal_properties = parent.thermal_properties;
        this->parent = &parent;
    }
};

#ifndef JARALYN_IDENTITY_HXX
#define JARALYN_IDENTITY_HXX

using j_id_t = uint64_t;
using j_static_id_t = uint64_t;

/**
 * @brief Atomic, auto incrementing entity id generator
 */
template<typename identifyable>
class j_identity {
    j_id_t id_;
public:
    j_identity() {
        static std::atomic<j_id_t> next_id { 0 };
        id_ = next_id++;
    }

    j_id_t id() const {
        return id_;
    }
};

/**
 * @brief Provides identity generation for types
 */
template<typename identity_group>
class j_static_identity_provider {
private:
    static j_static_id_t next_;
public:
    /**
     * @brief Generates an id for the type t within the group identity_group
     */
    template<typename t>
    [[nodiscard]] static const j_static_id_t generate() {
        static const j_static_id_t id { next_++ };
        return id;
    }
};

#endif

#ifndef JARALYN_IDENTITY_HXX
#define JARALYN_IDENTITY_HXX

using j_id = uint32_t;

/**
 * @brief Atomic, auto incrementing entity id generator
 */
template<class t>
class j_identity {
    j_id id_;
public:
    j_identity() {
        static std::atomic<j_id> next_id { 0 };
        id_ = next_id++;
    }

    uint32_t id() const noexcept {
        return id_;
    }
};

#endif

#include "object.hxx"

std::atomic<uint64_t> object::next_id_(0);

object::object() : id_(object::next_id_++) {
}

object_type_t object::type() const noexcept {
    return type_;
}

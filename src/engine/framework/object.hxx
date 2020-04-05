#ifndef JARALYN_OBJECT_HXX
#define JARALYN_OBJECT_HXX

class object {
private:
    static std::atomic<uint64_t> next_id_;
protected:
    const uint64_t id_;
    object_type_t type_ = 0;
public:
    object();
    virtual ~object() = default;
    object_type_t type() const noexcept;
};

#endif

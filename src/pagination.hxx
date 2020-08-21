#ifndef JARALYN_PAGINATION_HXX
#define JARALYN_PAGINATION_HXX

class j_pagination {
    uint32_t page_ { 0 };
    uint32_t pages_ { 0 };
    uint32_t page_size_ { 0 };
    uint32_t start_index_ { 0 };
    uint32_t stop_index_ { 0 };
    uint32_t max_index_ { 0 };
    uint32_t size_ { 0 };
public:
    constexpr void update(uint32_t size) {
        max_index_ = size ? size - 1 : 0;
        pages_ = page_size_ ? (size / page_size_) + !!(size % page_size_) : 0;
        size_ = size;
        if (pages_ && page_ > pages_ - 1) {
            page_ = pages_ - 1;
        }
        if (!page_size_ || !max_index_) {
            start_index_ = 0;
            stop_index_ = 0;
        } else {
            start_index_ = std::min(page_ * page_size_, max_index_);
            stop_index_ = std::min(start_index_ + (page_size_ - 1), max_index_);
        }
    }

    constexpr void set_page(uint32_t page) {
        page_ = page;
    }

    constexpr void set_page_by_index(uint32_t index) {
        if (!page_size_) {
            page_ = 0;
            return;
        }
        page_ = index / page_size_;
    }

    constexpr void set_page_size(uint32_t page_size) {
        page_size_ = page_size;
    }

    template<typename cont, typename each_fn>
    constexpr void each(const cont& container, each_fn fn) const {
        if (!size_) {
            return;
        }
        for (uint32_t i { start_index_ }, n { 0 }; i <= stop_index_; ++i, ++n) {
            fn(container[i], i, n);
        }
    }

    constexpr uint32_t page() const {
        return page_;
    }

    constexpr uint32_t page_size() const {
        return page_size_;
    }

    constexpr uint32_t pages() const {
        return pages_;
    }

    constexpr uint32_t start_index() const {
        return start_index_;
    }
    
    constexpr uint32_t stop_index() const {
        return stop_index_;
    }

    constexpr uint32_t max_index() const {
        return max_index_;
    }

    constexpr uint32_t remaining_entries() const {
        return max_index_ - stop_index_;
    }

    constexpr bool is_last_page() const {
        return max_index_ == stop_index_;
    }
};

#endif

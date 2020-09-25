#ifndef JARALYN_JOURNAL_HXX
#define JARALYN_JOURNAL_HXX

/**
 * @brief Journal of game events
 *
 * TODO: pagination, limits, filtering
 */
class j_journal {
private:
    std::vector<std::string> entries_;
public:
    template<typename... args>
    decltype(auto) emplace_back(args&&... entry_args) {
        return entries_.emplace_back(std::forward<args>(entry_args)...);
    }

    /**
     * @brief Returns all entries which should be displayed
     */
    [[nodiscard]] const std::vector<std::string>& entries() const;
};

#endif

#ifndef NOEDEN_LOG_XRAY_HXX
#define NOEDEN_LOG_XRAY_HXX

#include "xray.hxx"

class LogXray : public Xray {
public:
    void render() override;
private:
    // true if the following checkbox is currently checked
    bool follow_logs_ { true };
    // true if the 'Follow' button was pressed this frame
    bool follow_pressed_ { false };

    struct LogFilter {
        // current filter text input value
        std::string_view message_contains;
        // filter results after applying the filter
        Log::LogStore results;
    } filter_;

    /**
     * @brief Called as the user changes the filter input value
     */
    static i32 on_search(ImGuiInputTextCallbackData* data);

    void level_toggle();
    void log_filter();
    void log_table();
};

#endif

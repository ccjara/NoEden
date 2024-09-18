#ifndef NOEDEN_EXIT_MANAGER
#define NOEDEN_EXIT_MANAGER

class ExitManager {
public:
    /**
     * @brief Returns whether the game can exithout losing save data
     */
    bool can_exit() const;

    /**
     * @brief Returns whether the game should exit
     *
     * If true the game will exit on the next iteration of the main loop.
     */
    bool exit_requested() const;

    /**
     * @brief Requests the game to exit
     *
     * @param force If true the game will exit forcefully even if there is unsaved data
     * @return False if the game cannot exit due to unsaved data, true otherwise
     */
    bool request_exit(bool force);
private:
    bool can_exit_ = true;
    bool exit_requested_ = false;
};

#endif

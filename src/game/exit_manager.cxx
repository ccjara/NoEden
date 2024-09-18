#include "game/exit_manager.hxx"

bool ExitManager::can_exit() const {
    return can_exit_;
}

bool ExitManager::exit_requested() const {
    return exit_requested_;
}

bool ExitManager::request_exit(bool force) {
    // note: currently always returns true since there is no save game tracking yet
    if (can_exit_ || force) {
        exit_requested_ = true;
    }
    return exit_requested_;
}

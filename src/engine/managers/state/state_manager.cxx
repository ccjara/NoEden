#include "state_manager.hxx"

void state_manager::startup() {

}

void state_manager::shutdown() {

}

void state_manager::notify(change_notification&& n) {
    dispatcher_.enqueue<change_notification>(n);
}

void state_manager::distribute_changes() {
    dispatcher_.update();
}

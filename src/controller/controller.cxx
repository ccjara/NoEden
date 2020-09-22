#include "controller.hxx"

j_controller::~j_controller() {
    // clean up resources in case the controller has not been programmatically destroyed
    if (!destroyed_) {
        destroy();
    }
}

void j_controller::on_create() {

}

void j_controller::on_destroy() {

}

void j_controller::on_update(uint32_t delta_time) {

}

void j_controller::destroy() {
    destroyed_ = true;
    on_destroy();
}

#include "callback_store.hxx"

void CallbackStore::store(const CallbackInfo& info) {
    callbacks_[info.id] = info;
}

void CallbackStore::remove_by_callback_id(Id callback_id) {
    std::erase_if(callbacks_, [callback_id](const auto& iter) {
        return iter.first == callback_id;
    });
}

void CallbackStore::remove_by_script_id(Id script_id) {
    std::erase_if(callbacks_, [script_id](const auto& iter) {
        return iter.second.script_id == script_id;
    });
}

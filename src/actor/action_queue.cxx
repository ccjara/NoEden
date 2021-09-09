#include "action_queue.hxx"

void ActionQueue::process() {
    sort_by_speed();
    for (auto& action : actions_) {
        action->perform();
    }
    actions_.clear();
}

void ActionQueue::sort_by_speed() {
    std::sort(
        actions_.begin(),
        actions_.end(),
        [](const ActionPtr& a, const ActionPtr& b) {
            return a->speed > b->speed;
        }
    );
}

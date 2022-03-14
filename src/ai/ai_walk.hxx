#ifndef JARALYN_AI_WALK_HXX
#define JARALYN_AI_WALK_HXX

#include "ai_node.hxx"
#include "../actor/actor.hxx"
#include "../actor/action_queue.hxx"
#include "../actor/move_action.hxx"

class AiWalk : public AiNode {
public:
    AiWalk(Actor* actor, ActionQueue& action_queue) : 
        actor_ { actor },
        action_queue_ { action_queue } {
        assert(actor_);
    }

    void clear() override {
    }

    AiNodeState visit() override {
        static int dir = 0;
        if (!actor_) {
            return mod_state(AiNodeState::Failed);
        }
        auto pos { actor_->position };

        if (dir == 0) {
            --pos.y;
        } else if (dir == 1) {
            ++pos.x;
        } else if (dir == 2) {
            ++pos.y;
        } else if (dir == 3) {
            --pos.x;
        }
        ++dir %= 4;

        action_queue_.push<MoveAction>(actor_, pos);
        return mod_state(AiNodeState::Succeeded);
    }
private:
    Actor* actor_ { nullptr };
    ActionQueue& action_queue_;
};

#endif

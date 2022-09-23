#ifndef JARALYN_AI_WALK_HXX
#define JARALYN_AI_WALK_HXX

#include "ai_node.hxx"
#include "../actor/actor.hxx"
#include "../actor/move_action.hxx"
#include "../scene/scene.hxx"

class AiWalk : public AiNode {
public:
    AiWalk(Actor* actor) : actor_(actor) {
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

        Scene::create_action<MoveAction>(actor_, pos);
        return mod_state(AiNodeState::Succeeded);
    }
private:
    Actor* actor_ = nullptr;
};

#endif

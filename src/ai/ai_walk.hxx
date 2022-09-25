#ifndef JARALYN_AI_WALK_HXX
#define JARALYN_AI_WALK_HXX

#include "ai_node.hxx"
#include "../entity/entity.hxx"
#include "../entity/move_action.hxx"
#include "../scene/scene.hxx"

class AiWalk : public AiNode {
public:
    AiWalk(Entity* entity) : entity_(entity) {
        assert(entity_);
    }

    void clear() override {
    }

    AiNodeState visit() override {
        static int dir = 0;
        if (!entity_) {
            return mod_state(AiNodeState::Failed);
        }
        auto pos { entity_->position };

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

        Scene::create_action<MoveAction>(entity_, pos);
        return mod_state(AiNodeState::Succeeded);
    }
private:
    Entity* entity_ = nullptr;
};

#endif

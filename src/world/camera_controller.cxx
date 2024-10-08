#include "world/camera_controller.hxx"
#include "world/camera.hxx"
#include "world/world_spec.hxx"
#include "entity/entity.hxx"
#include "world/chunk.hxx"
#include "world/world_context.hxx"
#include "entity/entity_manager.hxx"

void CameraController::initialize(WorldContext* world_context) {
    world_context_ = world_context;
    assert(world_context);

    auto* events = world_context->events;

    world_updated_post_sub_ = events->on<WorldUpdatedPostEvent>(this, &CameraController::on_world_updated_post);
    key_down_sub_ = events->on<KeyDownEvent>(this, &CameraController::on_key_down);
    world_ready_sub_ = events->on<WorldReadyEvent>(this, &CameraController::on_world_ready);
    display_resized_sub_ = events->on<DisplayResizedEvent>(this, &CameraController::on_display_resized);
}

EventResult CameraController::on_world_updated_post(const WorldUpdatedPostEvent&) {
    if (!camera_) {
        return EventResult::Continue;
    }

    if (target_) {
        adjust(target_->position);
    }

    return EventResult::Continue;
}

Entity* CameraController::target() const {
    return target_;
}

EventResult CameraController::on_key_down(const KeyDownEvent& e) {
    // TODO: this later needs to be moved into a separate Ui controller which gets triggered by entering the "look" mode
    // right now we put it here to test camera and world generation before implementing Ui systems
    if (e.key == Key::Up) {
        target_ = nullptr;
        adjust(camera_->position + WorldPos(0, 0, -10));
        LOG_DEBUG("Camera now at {}", camera_->position.to_string());
    } else if (e.key == Key::Down) {
        target_ = nullptr;
        adjust(camera_->position + WorldPos(0, 0, 10));
        LOG_DEBUG("Camera now at {}", camera_->position.to_string());
    } else if (e.key == Key::Left) {
        target_ = nullptr;
        adjust(camera_->position + WorldPos(-10, 0, 0));
        LOG_DEBUG("Camera now at {}", camera_->position.to_string());
    } else if (e.key == Key::Right) {
        target_ = nullptr;
        adjust(camera_->position + WorldPos(10, 0, 0));
        LOG_DEBUG("Camera now at {}", camera_->position.to_string());
    } else if (e.key == Key::P) {
        set_target(world_context_->entity_manager->player());
    } else if (e.key == Key::KeyPadPlus) {
        target_ = nullptr;
        adjust(camera_->position + WorldPos(0, 1, 0));
        LOG_DEBUG("Camera now at {}", camera_->position.to_string());
    } else if (e.key == Key::KeyPadMinus) {
        target_ = nullptr;
        adjust(camera_->position + WorldPos(0, -1, 0));
        LOG_DEBUG("Camera now at {}", camera_->position.to_string());
    }
    return EventResult::Continue;
}

void CameraController::control(Camera* camera) {
    camera_ = camera;
}

void CameraController::adjust(const WorldPos& position) {
    if (!camera_) {
        return;
    }

    const i32 display_half_width = dispay_size_.x / 2;
    const i32 display_half_height = dispay_size_.y / 2;

    WorldPos camera_pos = position;

    if (camera_pos.x < display_half_width) {
        camera_pos.x = display_half_width;
    } else if (camera_pos.x > world_width_ - display_half_width) {
        camera_pos.x = world_width_ - display_half_width;
    }

    if (camera_pos.z < display_half_height) {
        camera_pos.z = display_half_height;
    } else if (camera_pos.z > world_height_ - display_half_height) {
        camera_pos.z = world_height_ - display_half_height;
    }

    camera_pos.y = std::clamp(camera_pos.y, 0, Chunk::CHUNK_DEPTH - 1);

    camera_->position = camera_pos;
}

EventResult CameraController::on_world_ready(const WorldReadyEvent& e) {
    world_width_ = e.world_spec->chunks_x() * Chunk::CHUNK_SIDE_LENGTH;
    world_height_ = e.world_spec->chunks_z() * Chunk::CHUNK_SIDE_LENGTH;
    return EventResult::Continue;
}

EventResult CameraController::on_display_resized(const DisplayResizedEvent& e) {
    dispay_size_ = e.size;
    return EventResult::Continue;
}

void CameraController::set_target(Entity* entity) {
    target_ = entity;
    if (target_) {
        adjust(target_->position);
    }
}

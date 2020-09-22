#include "unit_controller.hxx"

void j_unit_controller::on_create() {
    dispatcher_->sink<j_gathering_started_event>().connect<&j_unit_controller::on_gathering_started_event>(this);
}

void j_unit_controller::on_destroy() {
    dispatcher_->sink<j_gathering_started_event>().disconnect<&j_unit_controller::on_gathering_started_event>(this);
}

void j_unit_controller::on_gathering_started_event(const j_gathering_started_event& e) {
    auto registry { game->entities() };
    if (!registry->has<jc_inventory>(e.gatherer)) {
        LOG(ERROR) << "Entity " << static_cast<uint32_t> (e.gatherer) << " attempted to gather but has no item container";
        return;
    }
    auto& inventory { registry->get<jc_inventory>(e.gatherer) };
    auto item_group { registry->group<jc_item, jc_position>() };

    // first iteration: pick up first matching item or fail silently
    for (auto item : item_group) {
        const auto& position { item_group.get<jc_position>(item) };

        if (position.x == e.position.x && position.y == e.position.y) {
            inventory.put(0, item);
            registry->remove<jc_position>(item);

            dispatcher_->trigger<j_item_stored_event>(item);

            // may later pickup several items in one gathering attempt
            dispatcher_->trigger<j_gathering_completed_event>(
                e.gatherer,
                item,
                inventory.get_container(0)
            );
            break;
        }
    }
}

#include "unit_system.hxx"

void j_unit_system::on_load() {
    define_task<j_gathering_started_event, &j_unit_system::task_pickup_item>();
}

void j_unit_system::task_pickup_item(const j_gathering_started_event& e) {
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

        if (position == e.position) {
            inventory.items.push_back(item);
            registry->remove<jc_position>(item);

            dispatcher_->trigger<j_item_stored_event>(item);

            // may later pickup several items in one gathering attempt
            dispatcher_->trigger<j_gathering_completed_event>(
                e.gatherer,
                item,
                &inventory
            );
            break;
        }
    }
}

void j_unit_system::update(uint32_t delta_time) {
    queue_.update();
}

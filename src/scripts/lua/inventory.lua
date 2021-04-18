function on_update_inventory_window(window)
end

function on_load()
    local inventory_window = ui:create_window("inventory_window", "window");

    function on_inventory_view(owner)
        if owner then
            inventory_window:set_title(string.format(
                "Inventory of %s",
                entity.name(owner) or "Unknown"
            ));
        else
            inventory_window:set_title("Inventory");
        end
        inventory_window:show();
    end

    if inventory_window then
        inventory_window:set_title("Inventory");
        inventory_window:set_handler(on_update_inventory_window);
        inventory_window:move(0, 0);
        inventory_window:resize(20, 20);
        inventory_window:set_anchor_origin(anchor_origin.center);
    end

    script:on(event.inventory_view, on_inventory_view);
end

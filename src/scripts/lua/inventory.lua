function on_update_inventory_window(window)
end

function on_load()
    log:debug("on_load() - Creating inventory window");

    local inventory_window = ui:create_window("inventory_window");

    function on_inventory_view(owner)
        if owner then
            inventory_window:set_title(string.format(
                "Inventory of %s",
                scene:entity_name(owner)
            ));
        else
            inventory_window:set_title("Inventory");
        end
        inventory_window:show();
    end

    if not inventory_window then
        return;
    end

    inventory_window:set_title(string.format("Inventory of %s", scene:player_name() or "?"));
    -- inventory_window:set_handler(on_update_inventory_window);
    inventory_window:move(0, 0);
    inventory_window:resize(40, 20);
    inventory_window:set_align_x(AlignX.Center);
    inventory_window:set_align_y(AlignY.Center);
    inventory_window:set_anchor_origin(AnchorOrigin.Center);
    inventory_window:show();

    local text = ui:create_text("dummy_text");

    if not text then
        return;
    end;

    local inventory_lines = "";

    for i = 1,10,1
    do
        inventory_lines = inventory_lines..string.format("$c00ff00%3i$! %s$n", i, "Some Item");
    end

    text:set_parent(inventory_window);
    text:move(2, 2);
    text:resize(20, 20);
    text:anchor_to(inventory_window);
    text:set_text(inventory_lines);
    text:show();

    -- script:on(event.inventory_view, on_inventory_view);
end

function on_unload()
    log:debug("on_unload() - Destroying inventory window");
    ui:destroy_node("inventory_window");
end
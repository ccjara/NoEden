-- TODO: Move into library
AnchorOrigin = {
    Top = 0,
    TopRight = 1,
    Right = 2,
    BottomRight = 3,
    Bottom = 4,
    BottomLeft = 5,
    Left = 6,
    TopLeft = 7,
    Center = 8,
}

function on_update_inventory_window(window)
end

function on_load()
    log:debug("on_load() - Creating inventory window");

    local inventory_window = ui:create_window("inventory_window");

    function on_inventory_view(owner)
        if owner then
            inventory_window:set_title(string.format(
                "Inventory of %s",
                scene:actor_name(owner)
            ));
        else
            inventory_window:set_title("Inventory");
        end
        inventory_window:show();
    end

    if inventory_window then
        inventory_window:set_title("Inventory");
        -- inventory_window:set_handler(on_update_inventory_window);
        inventory_window:move(0, 0);
        inventory_window:resize(20, 20);
        inventory_window:set_anchor_origin(AnchorOrigin.Center);
        inventory_window:show();
    end

    -- script:on(event.inventory_view, on_inventory_view);
end

function on_unload()
    log:debug("on_unload() - Destroying inventory window");
    ui:destroy_window("inventory_window");
end
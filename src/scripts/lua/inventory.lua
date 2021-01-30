function on_update_inventory_window()
    -- display:text("Inventory", 0, 0, 0x00ff00)
end

function on_load(env)
    local inventory_window = ui:create_window("inventory");

    inventory_window:set_title("Inventory from Lua");
    inventory_window:set_handler(on_update_inventory_window);
    inventory_window:move(10, 10);
    inventory_window:resize(20, 20);
end

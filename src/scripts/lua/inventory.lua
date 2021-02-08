local int = 0;

function on_update_inventory_window(window)
    window:set_title(string.format("Title update %i", int));
    int = int + 1;
end

function on_load(env)
    local inventory_window = ui:create_window("inventory", "window");
    if inventory_window then
        inventory_window:set_title("Inventory from Lua!");
        inventory_window:set_handler(on_update_inventory_window);
        inventory_window:move(10, 10);
        inventory_window:resize(20, 20);
    end
end

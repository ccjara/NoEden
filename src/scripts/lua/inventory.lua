function on_render_inventory(display)
    display:text("Inventory", 0, 20, 1, 0)
end

function on_load(env)
    env:register_renderer("inventory", on_render_inventory)
end
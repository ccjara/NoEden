function on_run()
    local text = ui:create_text("test");

    text:resize(20, 20);
    text:set_text("Test");
    text:show();
end

function on_unload()
    ui:destroy_node("test");
end

function on_run()
    local text = ui:create_text("test");

    text:resize(20, 20);
    text:set_text("$cFF0000H$cFF7F00e$cFFFF00l$c00FF00l$c0000FFo$c4B0082 $c8B00FFW$cFF0000o$cFF7F00r$cFFFF00l$c00FF00d$c0000FF!");
    text:show();
end

function on_unload()
    ui:destroy_node("test");
end

#ifndef JARALYN_UI_PROXY_HXX
#define JARALYN_UI_PROXY_HXX

#include "../hud/ui_tree.hxx"

class j_ui_window_proxy {
public:
    j_ui_window_proxy(j_ui_window* window) : window_(window) {
        assert(window_);
    }

    void set_handler(luabridge::LuaRef ref) {
        if (!ref.isFunction()) {
            LOG(ERROR) << "REF NOT FUN";
            return;
        }
        window_->set_lua_ref(ref);
    }

    void set_title(const char *title) {
        window_->set_title(title);
    }

    void move(uint32_t x, uint32_t y) {
        window_->move({ x, y });
    }

    void resize(uint32_t width, uint32_t height) {
        window_->resize({ width, height });
    }
private:
    j_ui_window* window_;
};

class j_ui_proxy {
public:
    j_ui_proxy(j_ui_tree* tree);

    j_ui_window_proxy create_window(const char* id);
private:
    j_ui_tree* tree_ { nullptr };
};

#endif

#ifndef JARALYN_API_REGISTAR_HXX
#define JARALYN_API_REGISTAR_HXX

#include "../script_event.hxx"
#include "lua_api.hxx"

class ApiRegistrar {
public:
    explicit ApiRegistrar(entt::dispatcher& dispatcher);

    /**
     * @brief Constructs a lua api fragment in place
     * 
     * @see LuaApi
     */
    template<typename Api, typename... ApiArgs>
    void add_api(ApiArgs&&... api_args);
private:
    entt::dispatcher& dispatcher_;
    std::vector<std::unique_ptr<LuaApi>> apis_;

    void on_script_loaded(const ScriptLoadedEvent& e);
};

template<typename Api, typename... ApiArgs>
void ApiRegistrar::add_api(ApiArgs&&... api_args) {
    auto& api {
        apis_.emplace_back(new Api(std::forward<ApiArgs>(api_args)...))
    };
    api->dispatcher_ = &dispatcher_;
}

#endif

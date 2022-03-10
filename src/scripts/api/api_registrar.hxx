#ifndef JARALYN_API_REGISTAR_HXX
#define JARALYN_API_REGISTAR_HXX

#include "../script_event.hxx"
#include "lua_api.hxx"

class ApiRegistrar {
public:
    explicit ApiRegistrar(EventManager& events);

    /**
     * @brief Constructs a lua api fragment in place
     * 
     * @see LuaApi
     */
    template<typename Api, typename... ApiArgs>
    void add_api(ApiArgs&&... api_args);
private:
    EventManager& events_;
    std::vector<std::unique_ptr<LuaApi>> apis_;

    bool on_script_loaded(ScriptLoadedEvent& e);
};

template<typename Api, typename... ApiArgs>
void ApiRegistrar::add_api(ApiArgs&&... api_args) {
    auto& api {
        apis_.emplace_back(new Api(std::forward<ApiArgs>(api_args)...))
    };
    api->events_ = &events_;
}

#endif

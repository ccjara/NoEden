#ifndef NOEDEN_GAME_API_HXX
#define NOEDEN_GAME_API_HXX

#include "scripts/api/lua_api.hxx"

/**
 * @brief API to interact with the game on a high level
 */
class GameApi : public LuaApi {
public:
    void on_register(Script& script) override;
private:
    /**
     * @brief Requests the game to exit
     *
     * @see ExitManager::request_exit
     */
    bool exit(bool force) const;
};

#endif

#ifndef NOEDEN_GAME_HXX
#define NOEDEN_GAME_HXX

class ConfigManager;
class Scripting;
class Catalog;
class Platform;
class Input;
class Translator;
class Renderer;
class RealmManager;
class Ui;

#ifdef NOEDEN_XRAY
class XrayManager;
#endif

class Game {
public:
    static int start();

    ~Game();

private:
    /**
     * @brief Initializes the game and its subsystems
     */
    bool initialize();

    /**
     * @brief Starts the game
     */
    void run();

    /**
     * @brief Shuts down the game and its subsystems
     */
    void shutdown();

    std::unique_ptr<ConfigManager> config_manager_ = nullptr;
    std::unique_ptr<EventManager> events_ = nullptr;
    std::unique_ptr<Scripting> scripting_ = nullptr;
    std::unique_ptr<ServiceLocator> services_ = nullptr;
    std::unique_ptr<Catalog> catalog_ = nullptr;
    std::unique_ptr<Platform> platform_ = nullptr;
    std::unique_ptr<Input> input_ = nullptr;
    std::unique_ptr<Translator> t_ = nullptr;
    std::unique_ptr<Renderer> renderer_ = nullptr;
    std::unique_ptr<RealmManager> realms_ = nullptr;
    std::unique_ptr<Ui> ui_ = nullptr;

#ifdef NOEDEN_XRAY
    std::unique_ptr<XrayManager> xray_manager_ = nullptr;
#endif

    bool initialize_realms();
};

#endif

#pragma once

class ConditionResolver;
class ConfigManager;
class Scripting;
class Platform;
struct InputState;
class Translator;
class Renderer;
class RealmManager;
class ExitManager;
class ResourceManager;
class Ui;

#ifdef NOEDEN_XRAY
class XrayManager;
#endif

class Game {
public:
    static bool start();

    ~Game();

private:
    /**
     * @brief Initializes the game and its subsystems
     */
    bool initialize();

    /**
     * @brief Starts the game
     *
     * @return True if the game exited gracefully
     */
    bool run();

    /**
     * @brief Shuts down the game and its subsystems
     */
    void shutdown();

    std::unique_ptr<ResourceManager> res_ = nullptr;
    std::unique_ptr<ConditionResolver> condition_resolver_ = nullptr;
    std::unique_ptr<ExitManager> exit_manager_ = nullptr;
    std::unique_ptr<ConfigManager> config_manager_ = nullptr;
    std::unique_ptr<EventManager> events_ = nullptr;
    std::unique_ptr<Scripting> scripting_ = nullptr;
    std::unique_ptr<ServiceLocator> services_ = nullptr;
    std::unique_ptr<Platform> platform_ = nullptr;
    std::unique_ptr<InputState> input_ = nullptr;
    std::unique_ptr<Translator> t_ = nullptr;
    std::unique_ptr<Renderer> renderer_ = nullptr;
    std::unique_ptr<RealmManager> realms_ = nullptr;
    std::unique_ptr<ThreadPool> tasks_ = nullptr;
    std::unique_ptr<Ui> ui_ = nullptr;

#ifdef NOEDEN_XRAY
    std::unique_ptr<XrayManager> xray_manager_ = nullptr;
#endif

    bool initialize_realms();
};

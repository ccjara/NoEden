#pragma once

class ExitManager;

class Platform {
public:
    explicit Platform(EventManager* events, InputState* input, ExitManager* exit_manager);

    /**
     * @brief Initializes the platform (SDL, OpenGL, ImGui, etc.)
     *
     * @returns false if initialization failed.
     */
    bool initialize();

    /**
     * @brief Prepares a new loop iteration. Must be called at the beginning of the main loop.
     */
    void prepare();

    /**
     * @brief Presents the current frame. 
     * 
     * Must be called at the end of the loop after all drawing operations.
     */
    void present();

    /**
     * @brief Shuts down the platform.
     */
    void shutdown();

    glm::ivec2 window_size() const;

    ~Platform();

    Platform(const Platform&) = delete;
    Platform& operator=(const Platform&) = delete;
    Platform(Platform&&) = delete;
    Platform& operator=(Platform&&) = delete;
private:
    /**
     * @brief Processes incoming OS events
     */
    void process_events();

    bool sdl_initialized_ = false;
    EventManager* events_ = nullptr;
    SDL_Window* sdl_window_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;
 #ifdef NOEDEN_XRAY
    ImGuiContext* imgui_context_ = nullptr;
 #endif
    InputState* input_ = nullptr;
    ExitManager* exit_manager_ = nullptr;
};

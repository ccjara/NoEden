#include "platform/platform.hxx"
#include "platform/platform_event.hxx"
#include "game/exit_manager.hxx"

Platform::Platform(EventManager* events, Input *input, ExitManager* exit_manager) :
    events_(events),
    input_(input),
    exit_manager_(exit_manager) {
    assert(events_);
    assert(input_);
    assert(exit_manager_);
}

bool Platform::initialize() {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_ERROR("Failed to initialize SDL: {}", SDL_GetError());
        return false;
    }
    sdl_initialized_ = true;

    sdl_window_ = SDL_CreateWindow(
        "NoEden",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!sdl_window_) {
        LOG_ERROR("Failed to create SDL window: {}", SDL_GetError());
        return false;
    }

    gl_context_ = SDL_GL_CreateContext(sdl_window_);
    if (!gl_context_) {
        LOG_ERROR("Failed to create GL context: {}", SDL_GetError());
        return false;
    }
    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        LOG_ERROR("Failed to initialize glew");
        return false;
    }

#ifdef NOEDEN_XRAY
    imgui_context_ = ImGui::CreateContext();
    if (!imgui_context_) {
        LOG_ERROR("Could not create imgui context");
        return false;
    }
    ImGui_ImplSDL2_InitForOpenGL(sdl_window_, gl_context_);
    ImGui_ImplOpenGL3_Init();
#endif
    return true;
}

void Platform::prepare() {
    process_events();

#ifdef NOEDEN_XRAY
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
#endif
}

void Platform::present() {
#ifdef NOEDEN_XRAY
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

    SDL_GL_SwapWindow(sdl_window_);
}

void Platform::shutdown() {
#ifdef NOEDEN_XRAY
    if (imgui_context_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext(imgui_context_);
        imgui_context_ = nullptr;
    }
#endif

    if (gl_context_) {
        SDL_GL_DeleteContext(gl_context_);
        gl_context_ = nullptr;
    }

    if (sdl_window_) {
        SDL_DestroyWindow(sdl_window_);
        sdl_window_ = nullptr;
    }

    if (sdl_initialized_) {
        SDL_Quit();
        sdl_initialized_ = false;
    }
}

Platform::~Platform() {
    shutdown();
}

Vec2<u32> Platform::window_size() const {
    i32 w, h;
    SDL_GetWindowSize(sdl_window_, &w, &h);
    return { static_cast<u32>(w), static_cast<u32>(h) };
}

void Platform::process_events() {
    SDL_PumpEvents();

    SDL_Event e;
    while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_EventType::SDL_FIRSTEVENT, SDL_EventType::SDL_MOUSEWHEEL) != 0) {
#ifdef NOEDEN_XRAY
        ImGui_ImplSDL2_ProcessEvent(&e);
#endif

        switch (e.type) {
            case SDL_EventType::SDL_QUIT:
                exit_manager_->request_exit(false);
                break;
            case SDL_EventType::SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    events_->trigger<ResizeEvent>(Vec2<i32> {
                        static_cast<i32> (e.window.data1),
                        static_cast<i32> (e.window.data2)
                    });
                }
                break;
            case SDL_EventType::SDL_KEYDOWN:
                input_->set_key_pressed(static_cast<Key>(e.key.keysym.sym), true);
                break;
            case SDL_EventType::SDL_KEYUP:
                input_->set_key_pressed(static_cast<Key>(e.key.keysym.sym), false);
                break;
            case SDL_EventType::SDL_MOUSEBUTTONDOWN:
                input_->set_mouse_button_pressed(static_cast<MouseButton>(e.button.button), true);
                break;
            case SDL_EventType::SDL_MOUSEBUTTONUP:
                input_->set_mouse_button_pressed(static_cast<MouseButton>(e.button.button), false);
                break;
            case SDL_EventType::SDL_MOUSEMOTION:
                input_->set_mouse_position({ e.motion.x, e.motion.y });
                break;
        }
    }
}

#include "renderer.hxx"

Renderer::Renderer(Window& window, EventManager& dispatcher) :
    events_ { dispatcher },
    window_ { window } {

    events_.on<ResizeEvent>(this, &Renderer::on_resize);
    events_.on<ConfigUpdatedEvent>(this, &Renderer::on_config_updated);
}

Renderer::~Renderer() {
    reset();
}

void Renderer::initialize() {
    reset();

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    gl_context_ = SDL_GL_CreateContext(window_);

    if (gl_context_ == nullptr) {
        Log::error("Could not initialize opengl");
        std::abort();
    }
    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        Log::error("Could not initialize glew");
        std::abort();
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glyph
    glVertexAttribIPointer(0, 1, GL_INT, sizeof(Display::cell_type), nullptr);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Display::cell_type), reinterpret_cast<void*> (sizeof(Display::cell_type::glyph)));
    glEnableVertexAttribArray(1);

    text_shader_ = std::make_unique<TextShader>();

    set_viewport(window_.size());

    configure(Config());
}

void Renderer::render(const Scene& scene) {
    update_display(scene);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, view_port_.x, view_port_.y);

    text_shader_->use();

    glBindVertexArray(vao);

    const auto byte_size { display_.byte_size() };

    if (last_size_ == byte_size) {
        auto data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        std::memcpy(data, display_.data(), byte_size);

        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        // glBindBuffer(GL_ARRAY_BUFFER, vbo); // not necessary - autobound by vao
        glBufferData(GL_ARRAY_BUFFER, byte_size, display_.data(), GL_DYNAMIC_DRAW);

        last_size_ = byte_size;
    }
    glDrawArrays(GL_POINTS, 0, display_.cell_count());

    events_.trigger<PostRenderEvent>();

    SDL_GL_SwapWindow(window_);
}

void Renderer::update_display(const Scene& scene) {
    display_.reset();

    const auto& tiles { scene.read_tiles() };
    const auto& last_tile_index { display_.cell_count() };
    const auto scene_dim { tiles.dimensions() };
    const auto display_dim { display_.dimensions() };
    const Vec2<u32> tile_render_dim (
        std::min<u32>(scene_dim.x, display_dim.x),
        std::min<u32>(scene_dim.y, display_dim.y)
    );

    for (u32 y = 0; y < tile_render_dim.y; ++y) {
        for (u32 x = 0; x < tile_render_dim.x; ++x) {
            const Vec2<u32> tile_pos { x, y };
            const Tile* tile { tiles.at(tile_pos) };
            DisplayCell display_info { tile->display_info };
            if (!tile->revealed) {
                continue;
            }
            if (!tile->visited) {
                display_info.color = Color::mono(128);
            }
            display_.put(display_info, tile_pos);
        }
    }

    for (const auto& actor : scene.read_actors()) {
        // FIXME -> migrate grid to i32
        const Vec2<u32> pos {
            static_cast<u32> (actor->position.x >= 0 ? actor->position.x : 0),
            static_cast<u32> (actor->position.y >= 0 ? actor->position.y : 0)
        };
        if (!display_.in_bounds(pos)) {
            return; // do not render entities outside of view
        }
        const auto& display_info { actor->archetype->display_info };
        display_.put(
            DisplayCell(display_info.glyph, display_info.color),
            pos
        );
    }
}

void Renderer::set_viewport(Vec2<u32> size) {
    view_port_ = size;
    text_shader_->use_resolution(view_port_ / scaling_);
}

void Renderer::set_font(Texture* tex) {
    text_shader_->use_texture(tex);
}

void Renderer::set_glyph_size(Vec2<u32> glyph_size) {
    text_shader_->use_glyph_size(glyph_size);
}

void Renderer::set_scaling(u32 scaling) {
    scaling_ = scaling;
    text_shader_->use_resolution(view_port_ / scaling);
}

void Renderer::reset() {
    if (vao) {
        glDeleteBuffers(1, &vao);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
    if (gl_context_ != nullptr) {
        SDL_GL_DeleteContext(gl_context_);
        gl_context_ = nullptr;
    }
}

bool Renderer::on_resize(ResizeEvent& e) {
    set_viewport(e.size);
    adjust_display();
    return false;
}

void Renderer::configure(const Config& cfg) {
    cfg_ = cfg;

    const auto path_str { cfg_.font_texture_path.u8string() };
    if (!fs::exists(cfg_.font_texture_path)) {
        Log::error("Could not read text font at path {}", path_str);
        std::abort();
    }
    text_texture_.load(path_str);

    set_font(&text_texture_);
    set_glyph_size(cfg_.glyph_size);
    set_scaling(cfg_.scaling);
    adjust_display();
}

bool Renderer::on_config_updated(ConfigUpdatedEvent& e) {
    configure(e.next);
    return false;
}

void Renderer::adjust_display() {
    // calculate resolution
    const auto scaled_size { window_.size() / cfg_.scaling };
    // calculate how many cells will fit on the screen given that resolution
    const Vec2<u32> display_size {
        scaled_size.x / cfg_.glyph_size.x,
        scaled_size.y / cfg_.glyph_size.y
    };
    // resize and notify
    display_.resize(display_size);
    events_.trigger<DisplayResizedEvent>(display_size);
    Log::debug("Display resized to {}x{} cells", display_size.x, display_size.y);
}

SDL_GLContext Renderer::gl_context() const {
    return gl_context_;
}

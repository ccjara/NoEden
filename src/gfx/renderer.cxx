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
            if (tile->visited) {
                display_.put(tile->display_info, tile_pos);
            } else {
                display_.at(tile_pos)->color = Color::mono(128);
            }
        }
    }

    for (const auto& actor : scene.read_actors()) {
        if (!display_.in_bounds(actor->position)) {
            continue; // do not render entities outside of view
        }
        const auto tile { tiles.at(actor->position) };
        if (!tile || !tile->visited) {
            continue;
        }
        const auto& display_info { actor->archetype->display_info };
        display_.put(
            DisplayCell(display_info.glyph, display_info.color),
            actor->position
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

std::array<float, 4> Renderer::calculate_glyph_uv(u32 glyph) const {
    const Vec2<float> glyph_size = {
        static_cast<float>(text_shader_->glyph_size().x),
        static_cast<float>(text_shader_->glyph_size().y)
    };
    const Vec2<float> tex_size = {
        static_cast<float>(text_shader_->texture_size().x),
        static_cast<float>(text_shader_->texture_size().y)
    };
    const int chars_per_row_in_texture = static_cast<i32>(tex_size.x / glyph_size.x);

    const Vec2<float> tex_origin {
        static_cast<float>(glyph % chars_per_row_in_texture) * glyph_size.x,
        static_cast<float>(std::floor(glyph / chars_per_row_in_texture)) * glyph_size.y
    };

    const float u1 = 1.0f / tex_size.x * tex_origin.x;
    const float u2 = 1.0f / tex_size.x * (tex_origin.x + glyph_size.x);
    const float v1 = 1.0f / tex_size.y * tex_origin.y;
    const float v2 = 1.0f / tex_size.y * (tex_origin.y + glyph_size.y);

    return std::array<float, 4> { u1, v1, u2, v2 };
}

GLuint Renderer::text_texture() const {
    return text_texture_.id();
}

float Renderer::glyph_aspect_ratio() const {
    return static_cast<float>(
        text_shader_->glyph_size().x
    ) / text_shader_->glyph_size().y;
}

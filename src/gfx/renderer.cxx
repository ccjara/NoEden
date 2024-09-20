#include "gfx/renderer.hxx"
#include "gfx/gfx_event.hxx"
#include "resource/resource_manager.hxx"

Renderer::Renderer(EventManager* events, ResourceManager* res) : events_(events), res_(res) {
    assert(events_);
    assert(res_);

    resize_sub_ = events_->on<ResizeEvent>(this, &Renderer::on_resize);
    config_updated_sub_ = events_->on<ConfigUpdatedEvent>(this, &Renderer::on_config_updated);
}

Renderer::~Renderer() {
    if (vao) {
        glDeleteBuffers(1, &vao);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
}

bool Renderer::initialize() {
    if (initialized_) {
        LOG_ERROR("Renderer already initialized");
        return false;
    }

    shader_ = res_->shader("TEXT");

    if (!shader_) {
        return false;
    }

    glGenVertexArrays(1, &vao);

    if (glGetError() != GL_NO_ERROR) {
        LOG_ERROR("Failed to generate VAO");
        return false;
    }

    glGenBuffers(1, &vbo);

    if (glGetError() != GL_NO_ERROR) {
        LOG_ERROR("Failed to generate VBO");
        return false;
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glyph
    glVertexAttribIPointer(0, 1, GL_INT, sizeof(Display::cell_type), nullptr);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Display::cell_type), reinterpret_cast<void*> (sizeof(Display::cell_type::glyph)));
    glEnableVertexAttribArray(1);

    if (glGetError() != GL_NO_ERROR) {
        LOG_ERROR("Failed to initialize vertex attributes");;
        return false;
    }

    configure(Config());

    initialized_ = true;
    return true;
}

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    shader_->use();
    text_texture_.bind();

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    for (auto& layer : layers_) {
        const auto byte_size { layer.byte_size() };

        if (last_size_ == layer.byte_size()) {
            glBufferSubData(GL_ARRAY_BUFFER, 0, byte_size, layer.data());
        } else {
            glBufferData(GL_ARRAY_BUFFER, byte_size, layer.data(), GL_DYNAMIC_DRAW);

            last_size_ = byte_size;
        }
        glDrawArrays(GL_POINTS, 0, layer.cell_count());
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::set_viewport(Vec2<u32> size) {
    view_port_ = size;
    glViewport(0, 0, view_port_.x, view_port_.y);

    shader_->set_uniform("u_resolution", view_port_ / scaling_);
}

void Renderer::set_glyph_size(Vec2<u32> glyph_size) {
    glyph_size_ = glyph_size;
    shader_->set_uniform("u_glyph_size", glyph_size_);
}

void Renderer::set_scaling(u32 scaling) {
    scaling_ = scaling;
    shader_->set_uniform("u_resolution",  view_port_ / scaling_);
}

EventResult Renderer::on_resize(const ResizeEvent& e) {
    set_viewport(e.size);
    adjust_display();
    return EventResult::Continue;
}

bool Renderer::configure(const Config& cfg) {
    const auto path_str = cfg.font_texture_path.string();

    if (!fs::exists(cfg.font_texture_path)) {
        LOG_ERROR("Could not read text font at path {}", path_str);
        return false;
    }
    if (!text_texture_.load(path_str)) {
        LOG_ERROR("Could not load text texture at path {}", path_str);
        return false;
    }
    cfg_ = cfg;
    shader_->use();
    shader_->set_uniform("u_tex_size", text_texture_.size());
    set_glyph_size(cfg.glyph_size);
    set_scaling(cfg.scaling);
    adjust_display();
    return true;
}

EventResult Renderer::on_config_updated(const ConfigUpdatedEvent& e) {
    configure(e.next);
    return EventResult::Continue;
}

void Renderer::adjust_display() {
    // calculate resolution
    const auto scaled_size { view_port_ / cfg_.scaling };
    // calculate how many cells will fit on the screen given that resolution
    const Vec2<u32> display_size {
        scaled_size.x / cfg_.glyph_size.x,
        scaled_size.y / cfg_.glyph_size.y
    };
    // resize and notify
    for (auto& layer : layers_) {
        layer.resize(display_size);
    }

    events_->trigger<DisplayResizedEvent>(display_size);
    LOG_DEBUG("Display resized to {}x{} cells", display_size.x, display_size.y);
}

std::array<float, 4> Renderer::calculate_glyph_uv(u32 glyph) {
    const Vec2<float> glyph_size = {
        static_cast<float>(glyph_size_.x),
        static_cast<float>(glyph_size_.y)
    };
    const Vec2<float> tex_size = {
        static_cast<float>(text_texture_.size().x),
        static_cast<float>(text_texture_.size().y)
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

    return std::array { u1, v1, u2, v2 };
}

GLuint Renderer::text_texture() const {
    return text_texture_.id();
}

float Renderer::glyph_aspect_ratio() const {
    if (glyph_size_.y == 0) {
        return 0;
    }

    return static_cast<float>(glyph_size_.x) / static_cast<float>(glyph_size_.y);
}

Display& Renderer::display() {
    return layers_[0];
}

Display& Renderer::ui_layer() {
    return layers_[1];
}

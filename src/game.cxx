#include "game.hxx"

game::game() :
    layer_ptr(std::make_unique<layer>(size<uint32_t> { 100, 75 })),
    sdl_event_sink(sdl_event_signal) {
    gen.generate_test_arena(*layer_ptr);
}

game::~game() noexcept {
    sdl_event_sink.disconnect();
}

void game::setup() {
    fb = std::move(std::make_unique<frame_buffer>());
}

void game::handle_sdl_event(const SDL_Event& e) {
    sdl_event_signal.publish(e);

    if (e.type == SDL_MOUSEMOTION) {
        mouse_x = e.motion.x;
        mouse_y = e.motion.y;
        return;
    }

    auto upd = [this]() {
        layer_ptr->clear();
        gen.generate(*layer_ptr, conf);
        world_needs_update = true;
    };

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F5) {
        world_needs_update = true;
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_TAB) {
        upd();
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_PLUS) {
        conf.max_room_attempts++;
        upd();
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_MINUS && conf.max_room_attempts > 0) {
        conf.max_room_attempts--;
        upd();
    }
}

void game::render(renderer& rnd) {
    fps.pre_render();

    render_world(rnd);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 800, 600, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, 800, 600);
    glScalef(1.0f, 1.0f, 1.0f);

    rnd.text->begin();

    // rnd.text->render_text(test->get_text(), Vector2<GLfloat>(10, 10));

    char buf[255];
    std::sprintf(
        buf,
        "fps: %u || m: (%i, %i) || max attempts: %u",
        fps.get_fps(),
        mouse_x,
        mouse_y,
        conf.max_room_attempts
    );
    rnd.text->render_text(buf, vector2<GLfloat>{ 10, 10 });
    rnd.text->end();

    glBindTexture(GL_TEXTURE_2D, fb->get_texture());
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

    const auto xoff { 0 };
    const auto yoff { 32 };
    // top left
    glTexCoord2i(0, 0);
    glVertex2i(0 + xoff, 0 + yoff);
    // top right
    glTexCoord2i(1, 0);
    glVertex2i(512 + xoff, 0 + yoff);
    // bottom right
    glTexCoord2i(1, -1);
    glVertex2i(512 + xoff, 512 + yoff);
    // bottom left
    glTexCoord2i(0, -1);
    glVertex2f(0 + xoff, 512 + yoff);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void game::render_world(const renderer& rnd) {
    fb->bind();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 512, 512, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, 512, 512);

    if (world_needs_update) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        rnd.text->begin();

        const auto& ls { layer_ptr->get_size() };

        size_t i = 0;

        glColor3ub(255, 0, 0);
        for (const auto& o : layer_ptr->objects) {
            if (!o.is_void) {
                unsigned char symbol;

                if (o.debug_char != 0) {
                    symbol = static_cast<unsigned char> (o.debug_char);
                } else if (o.is_solid) {
                    symbol = static_cast<unsigned char> (0xDB);
                } else {
                    symbol = static_cast<unsigned char> ('.');
                }
                rnd.text->render_char(
                    symbol,
                    vector2<GLfloat> {
                        static_cast<GLfloat>(i % ls.width) * 8,
                        static_cast<GLfloat>(i / ls.width) * 12
                    }
                );
            }
            i++;
        }
        rnd.text->end();

        world_needs_update = false;
    }
    fb->unbind();
}
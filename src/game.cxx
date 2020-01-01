#include "game.hxx"

game::game() :
    layer_ptr(std::make_unique<layer>(vector2<uint32_t> {50, 50 })),
    test(std::make_unique<text_tester>()),
    sdl_event_sink(sdl_event_signal) {
    max_depth = 0;

    sdl_event_sink.connect<&text_tester::on_sdl_event>(test.get());
}

game::~game() noexcept {
    sdl_event_sink.disconnect();
}

void game::handle_sdl_event(const SDL_Event& e) {
    sdl_event_signal.publish(e);

    if (e.type == SDL_MOUSEMOTION) {
        return;
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_TAB) {
        gen.generate(*layer_ptr, max_depth);
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_PLUS) {
        max_depth++;
        gen.generate(*layer_ptr, max_depth);
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_MINUS) {
        max_depth--;
        gen.generate(*layer_ptr, max_depth);
    }
}

void game::render(const renderer& rnd) {
    auto leafCount { 0u };
    std::function<void(const partition&)> rParts = [&](const partition& part) {
        if (part.first && part.second) {
            rParts(*part.first);
            rParts(*part.second);
            return;
        }
        leafCount++;
        glColor3ub(leafCount * 15, leafCount * 15, leafCount * 15);
        glRecti(part.pos.x * 8, part.pos.y * 12, (part.pos.x + part.size.x) * 8, (part.pos.y + part.size.y) * 12);
    };
    rParts(gen.test_partition);

    rnd.text->begin();

    glColor4ub(0, 255, 0, 128);

    size_t i = 0;
    const auto dim = layer_ptr->get_dimensions();
    for (const auto& o : layer_ptr->data) {
        if (o != nullptr && !o->is_void) {
            unsigned char symbol;

            if (o->is_solid) {
                glColor3ub(255, 0, 0);
                symbol = static_cast<unsigned char> (0xDB);
            } else {
                symbol = static_cast<unsigned char> ('.');
            }
            rnd.text->render_char(
                symbol,
                vector2<GLfloat> {
                    static_cast<GLfloat>(i % dim.x) * 8,
                    static_cast<GLfloat>(i / dim.x) * 12
                }
            );
        }
        i++;
    }

    // rnd.text->render_text(test->get_text(), Vector2<GLfloat>(10, 10));

    rnd.text->end();
}
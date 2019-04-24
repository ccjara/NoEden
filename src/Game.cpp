#include "stdafx.h"

Game::Game() :
    mainMenu(sdlEventManager),
    test{ std::make_unique<TextTester>(sdlEventManager) },
    layer{ std::make_unique<Layer>(Vector2<uint32_t>(100, 100)) },
    entities{ std::make_unique<Entities>() }
{
    entities->create<Wall>();
    entities->create<Wall>();
    entities->create<Wall>();
    entities->create<Wall>();
}

void Game::handleSdlEvent(const SDL_Event& e)
{
    if (e.type == SDL_MOUSEMOTION) {
        return;
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_TAB) {
        RoomGen gen;
        layer->clear();
        gen.generate(*layer);
    }
    sdlEventManager.trigger(e);
}

void Game::render(const Renderer& renderer)
{
    Vector2<GLfloat> p{ 10.0f, 10.0f };

    // TODO: improve readability, e.g. "The [c:ff0000ff]quick[/] ..."
    renderer.text->begin();

    size_t i = 0;
    const auto dim = layer->getDimensions();
    for (const auto& o : layer->data) {
        if (o != nullptr && o->isSolid) {
            renderer.text->renderChar(0xDB, Vector2<GLfloat>((i % dim.x) * 8, (i / dim.x) * 12));
        }
        i++;
    }

    /*
    std::string text(
    "The |cff0000ffquick |c00ff00fffox|r jumps |cffff00ffover|r the lazy dog|r|n\
    The |cffffff22clunky|r cat collides with the |cffffff22fat|r boar|n\
    This|nText|n|n|nHas |c00ff00ff4|r newlines in total.|n\
    |cff0000ffHey||Pipe"
    );
    */
    // renderer.text->renderText(test->getText(), p);
    renderer.text->end();
}
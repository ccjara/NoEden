#include "stdafx.h"

Renderer::Renderer() : text{ std::make_shared<TextRenderer>() }
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

Renderer::~Renderer()
{
    SDL_GL_DeleteContext(glContext);
}

void Renderer::bind(std::shared_ptr<Window> w)
{
    window = w;
    glContext = SDL_GL_CreateContext(window->getHandle());
    SDL_GL_SetSwapInterval(1);

    text->load();
}

void Renderer::startRendering()
{
    auto wSize = window->getSize();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, static_cast<GLsizei> (wSize.x), static_cast<GLsizei> (wSize.y));
    glOrtho(0.0f, static_cast<GLdouble> (wSize.x), static_cast<GLdouble> (wSize.y), 0.0f, 0.0f, 1.0f);
    // glTranslatef(0.5f, 0.5f, 0.0f);
    glScalef(2.0f, 2.0f, 1.0f);
}

void Renderer::finishRendering()
{
    SDL_GL_SwapWindow(window->getHandle());
}

void Renderer::run(Entities *entities) {
    startRendering();

    

    finishRendering();
}
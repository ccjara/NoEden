#ifndef __RENDERER_H__
#define __RENDERER_H__

class Renderer {
private:
    SDL_GLContext glContext;
    std::shared_ptr<Window> window;
public:
    const std::shared_ptr<TextRenderer> text;

    Renderer();
    ~Renderer();

    void bind(std::shared_ptr<Window> w);
    void startRendering();
    void finishRendering();
    void run(Entities *entities);
};

#endif

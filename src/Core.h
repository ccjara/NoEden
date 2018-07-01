#ifndef __CORE_H__
#define __CORE_H__

class Core
{
private:
    std::shared_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Game> game;

    void processEvents();
    void render();
    bool running;
public:
    Core();
    ~Core();
    void run();
    void shutdown() noexcept;
};

#endif

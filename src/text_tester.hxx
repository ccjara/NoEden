#ifndef JARALYN_TEXT_TESTER_HXX
#define JARALYN_TEXT_TESTER_HXX

class text_tester {
private:
    std::string input;
public:
    text_tester();
    ~text_tester() noexcept;

    void on_sdl_event(const SDL_Event& e);

    const char *get_text() const;
};

#endif

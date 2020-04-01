#ifndef JARALYN_TEXT_RENDERER_HXX
#define JARALYN_TEXT_RENDERER_HXX

class text_renderer
{
private:
    enum modifier : char {
        Color = 'c',
        Pipe = '|',
        NewLine = 'n',
        Reset = 'r',
    };

    GLuint texture;
    float_t tex_width;
    float_t tex_height;
    int32_t char_width;
    int32_t char_height;

    std::vector<SDL_Color> color_stack;
public:
    text_renderer();
    ~text_renderer();

    void load();
    void begin() const;
    void end() const;
    void render_text(const std::string& text, const vector2<GLfloat>& p);
    void render_char(char c, const vector2<GLfloat>& p);
};

#endif

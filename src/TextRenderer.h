#ifndef __TEXT_RENDERER_H__
#define __TEXT_RENDERER_H__

struct Modifier {
    uint32_t start;
    uint32_t end;
    SDL_Color color;
};

class TextRenderer
{
private:
    GLuint texture;
    float_t textureWidth;
    float_t textureHeight;
    int32_t charWidth;
    int32_t charHeight;

    std::vector<SDL_Color> colorStack;
public:
    TextRenderer();
    ~TextRenderer();

    void load();
    void begin() const;
    void end() const;
    void renderText(const std::string& text, const Vector2<GLfloat>& p);
    void renderChar(const char c, const Vector2<GLfloat>& p);
};

#endif

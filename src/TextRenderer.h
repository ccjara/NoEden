#ifndef __TEXT_RENDERER_H__
#define __TEXT_RENDERER_H__

class TextRenderer
{
private:
    GLuint texture;
    float_t textureWidth;
    float_t textureHeight;
    int32_t charWidth;
    int32_t charHeight;
public:
    TextRenderer();
    ~TextRenderer();

    void load();
    void begin() const;
    void end() const;
    void renderText(const std::string& text, const Vector2<GLfloat>& p) const;
    void renderChar(const char c, const Vector2<GLfloat>& p) const;
};

#endif

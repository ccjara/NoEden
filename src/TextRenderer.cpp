#include "stdafx.h"

TextRenderer::TextRenderer() :
    charWidth{ 8 },
    charHeight{ 12 }
{
}

TextRenderer::~TextRenderer()
{
    glDeleteTextures(1, &texture);
}

void TextRenderer::load()
{
    auto textSurface = SDL_LoadBMP("font.bmp");
    if (!textSurface) {
        LOG(ERROR) << "Could not load font bitmap (" << SDL_GetError() << ")";
        throw;
    }

    glGenTextures(1, &texture);
    auto glError = glGetError();
    if (glError) {
        LOG(ERROR) << "Could not initialize text texture (" << glError << ")";
        throw;
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    if (glError = glGetError()) {
        LOG(ERROR) << "Could not bind text texture (" << glError << ")";
        throw;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        static_cast<GLsizei> (textSurface->w),
        static_cast<GLsizei> (textSurface->h),
        0,
        GL_BGRA,
        GL_UNSIGNED_BYTE,
        textSurface->pixels
    );
    if (glError = glGetError()) {
        LOG(ERROR) << "Could not generate texture from text surface (" << glError << ")";
        throw;
    }

    glError = glGetError();
    if (glError) {
        LOG(ERROR) << "Could not map text surface to texture (" << glError << ")";
        throw;
    }

    textureWidth = static_cast<float_t> (textSurface->w);
    textureHeight = static_cast<float_t> (textSurface->h);

    SDL_FreeSurface(textSurface);
}

void TextRenderer::begin() const
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
}

void TextRenderer::end() const
{
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void TextRenderer::renderText(const std::string& text, const Vector2<GLfloat>& p)
{
    if (!text.length()) {
        return;
    }

    colorStack = std::vector<SDL_Color>{ { 255, 255, 255, 255 } };
    glColor4ub(255, 255, 255, 255);

    auto offset{ p };
    auto maxIndex = text.length() - 1;
    
    for (size_t index = 0; index <= maxIndex; index++) {
        if (index != maxIndex && text[index] == Modifier::Pipe) {
            index++;

            if (index >= maxIndex) { // ignore modifiers at the end of the string
                return;
            }

            switch (text[index]) {
            case Modifier::Reset:
                if (!colorStack.size()) {
                    LOG(ERROR) << "Color stack would be empty if modifier had been popped - ignoring";
                    continue;
                }
                colorStack.pop_back();
                auto color = colorStack.back();
                glColor4ub(color.r, color.g, color.b, color.a);
                continue;
            case Modifier::NewLine:
                offset.x = p.x;
                offset.y += charHeight;
                continue;
            case Modifier::Color: {
                const auto channelLength = 8; // wrap around RRGGBBAA
                const auto indexColorStart = index + 1;
                const auto indexColorEnd = indexColorStart + channelLength - 1;
                if (indexColorStart > maxIndex || indexColorEnd > maxIndex) {
                    LOG(ERROR) << "End of string found, expected color code";
                    continue;
                }
                const auto colorStr = text.substr(indexColorStart, channelLength);
                const auto colorNum = std::stoul(colorStr, nullptr, 16);

                SDL_Color color{
                    (colorNum >> 24) & 0xFF,
                    (colorNum >> 16) & 0xFF,
                    (colorNum >> 8) & 0xFF,
                    colorNum & 0xFF,
                };

                colorStack.push_back(color);
                glColor4ub(color.r, color.g, color.b, color.a);

                index = indexColorEnd;
                continue;
            }
            case Modifier::Pipe:
                break; // escaped pipe (||)
            default:
                LOG(WARNING) << "Unknown modifier '" << text[index] << '"';
                continue;
            }
        }

        renderChar(text[index], offset);
        offset.x += charWidth;
    }
}

void TextRenderer::renderChar(const char c, const Vector2<GLfloat>& p)
{
    const uint32_t charsPerRow{ 16 };
    const auto textureX = static_cast<float_t> (c % charsPerRow) * charWidth;
    const auto textureY = static_cast<float_t> (c / charsPerRow) * charHeight;

    const GLfloat u1{ 1.0f / textureWidth * textureX };
    const GLfloat u2{ 1.0f / textureWidth * (textureX + charWidth) };
    const GLfloat v1{ 1.0f / textureHeight * textureY };
    const GLfloat v2{ 1.0f / textureHeight * (textureY + charHeight) };

    // top left
    glTexCoord2f(u1, v1);
    glVertex2f(p.x, p.y);
    // top right
    glTexCoord2f(u2, v1);
    glVertex2f(p.x + charWidth, p.y);
    // bottom right
    glTexCoord2f(u2, v2);
    glVertex2f(p.x + charWidth, p.y + charHeight);
    // bottom left
    glTexCoord2f(u1, v2);
    glVertex2f(p.x, p.y + charHeight);
}

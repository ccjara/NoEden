#include "text_renderer.hxx"

text_renderer::text_renderer() :
    char_width{ 8 },
    char_height{ 12 }
{
}

text_renderer::~text_renderer()
{
    glDeleteTextures(1, &texture);
}

void text_renderer::load()
{
    auto text_surface = SDL_LoadBMP("font.bmp");
    if (!text_surface) {
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
    glError = glGetError();
    if (glError) {
        LOG(ERROR) << "Could not bind text texture (" << glError << ")";
        throw;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        static_cast<GLsizei> (text_surface->w),
        static_cast<GLsizei> (text_surface->h),
        0,
        GL_BGRA,
        GL_UNSIGNED_BYTE,
        text_surface->pixels
    );
    glError = glGetError();
    if (glError) {
        LOG(ERROR) << "Could not generate texture from text surface (" << glError << ")";
        throw;
    }
    tex_width = static_cast<float_t> (text_surface->w);
    tex_height = static_cast<float_t> (text_surface->h);

    SDL_FreeSurface(text_surface);
}

void text_renderer::begin() const
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
}

void text_renderer::end() const
{
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// FIXME: crashes in |c
void text_renderer::render_text(const std::string& text, const vector2<GLfloat>& p)
{
    if (!text.length()) {
        return;
    }

    color_stack = std::vector<SDL_Color>{{ 255, 255, 255, 255 } };
    glColor4ub(255, 255, 255, 255);

    auto offset{ p };
    auto maxIndex = text.length() - 1;
    
    for (size_t index = 0; index <= maxIndex; index++) {
        if (index != maxIndex && text[index] == modifier::Pipe) {
            index++;

            if (index >= maxIndex) { // ignore modifiers at the end of the string
                return;
            }

            switch (text[index]) {
            case modifier::Reset: {
                if (color_stack.empty()) {
                    LOG(ERROR) << "Color stack would be empty if modifier had been popped - ignoring";
                    continue;
                }
                color_stack.pop_back();
                auto color = color_stack.back();
                glColor4ub(color.r, color.g, color.b, color.a);
                continue;
            }
            case modifier::NewLine:
                offset.x = p.x;
                offset.y += char_height;
                continue;
            case modifier::Color: {
                const auto channel_len = 8; // wrap around RRGGBBAA
                const auto ix_color_start = index + 1;
                const auto ix_color_end = ix_color_start + channel_len - 1;
                if (ix_color_start > maxIndex || ix_color_end > maxIndex) {
                    LOG(ERROR) << "End of string found, expected color code";
                    continue;
                }
                const auto color_str = text.substr(ix_color_start, channel_len);
                const auto color_num = std::stoul(color_str, nullptr, 16);

                SDL_Color color {
                    static_cast<uint8_t> ((color_num >> 24) & 0xFF),
                    static_cast<uint8_t> ((color_num >> 16) & 0xFF),
                    static_cast<uint8_t> ((color_num >> 8) & 0xFF),
                    static_cast<uint8_t> (color_num & 0xFF),
                };

                color_stack.push_back(color);
                glColor4ub(color.r, color.g, color.b, color.a);

                index = ix_color_end;
                continue;
            }
            case modifier::Pipe:
                break; // escaped pipe (||)
            default:
                LOG(WARNING) << "Unknown modifier '" << text[index] << '"';
                continue;
            }
        }

        render_char(text[index], offset);
        offset.x += char_width;
    }
}

void text_renderer::render_char(char c, const vector2<GLfloat>& p)
{
    const uint32_t chars_per_row { 16 };
    const auto uc = static_cast<unsigned char> (c);
    const auto textureX = static_cast<float_t> (uc % chars_per_row) * char_width;
    const auto textureY = static_cast<float_t> (uc / chars_per_row) * char_height;

    const GLfloat u1 { 1.0f / tex_width * textureX };
    const GLfloat u2 { 1.0f / tex_width * (textureX + char_width) };
    const GLfloat v1 { 1.0f / tex_height * textureY };
    const GLfloat v2 { 1.0f / tex_height * (textureY + char_height) };

    // top left
    glTexCoord2f(u1, v1);
    glVertex2f(p.x, p.y);
    // top right
    glTexCoord2f(u2, v1);
    glVertex2f(p.x + char_width, p.y);
    // bottom right
    glTexCoord2f(u2, v2);
    glVertex2f(p.x + char_width, p.y + char_height);
    // bottom left
    glTexCoord2f(u1, v2);
    glVertex2f(p.x, p.y + char_height);
}

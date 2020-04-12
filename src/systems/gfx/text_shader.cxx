#include "text_shader.hxx"

void text_shader::load(texture* tex) {
    assert(tex);

    const std::string vss = R"RAW(
#version 330 core
layout (location = 0) in float glyph;
layout (location = 1) in vec3 color;

out v_struct {
    float glyph;
    vec3 color;
} vertex;

void main() {
    vertex.glyph = glyph;
    vertex.color = color;
}
)RAW";

    const std::string gss = R"RAW(
#version 330 core
 
layout (points) in;
layout (triangle_strip, max_vertices=4) out;

uniform vec2 u_view_port;
uniform vec2 u_glyph_size;
uniform vec2 u_tex_size;

in v_struct {
    float glyph;
    vec3 color;
} vertex[];

out f_struct {
    vec3 color;
    vec2 tex_coord;
} fragment;

void ndc(inout vec2 p) { // view port coords to npc coordinates
    p.x = 2.0 * p.x / u_view_port.x - 1.0;
    p.y = 1.0 - 2.0 * p.y / u_view_port.y;
}

void main() {
    // runs once for each vertex given since we pass in points
    vec3 color = vertex[0].color;
    float glyph = vertex[0].glyph;
    float cycle = gl_PrimitiveIDIn; // this saves us a counter var

    float chars_per_line = u_view_port.x / u_glyph_size.x;
    float chars_per_row_in_texture = u_tex_size.x / u_glyph_size.x;

    // pixel origin in texture
    vec2 tex_origin = vec2(
        mod(glyph, chars_per_row_in_texture) * u_glyph_size.x,
        floor(glyph / chars_per_row_in_texture) * u_glyph_size.y
    );

    // calculate tex coords for the letter box
    float u1 = 1.0 / u_tex_size.x * tex_origin.x;
    float u2 = 1.0 / u_tex_size.x * (tex_origin.x + u_glyph_size.x);
    float v1 = 1.0 / u_tex_size.y * tex_origin.y;
    float v2 = 1.0 / u_tex_size.y * (tex_origin.y + u_glyph_size.y);
    
    // pixel origin in 2d plane (reused across vertices)
    vec2 origin = vec2(
        u_glyph_size.x * mod(cycle, chars_per_line),
        floor(cycle / chars_per_line) * u_glyph_size.y
    );
    vec2 position;

    // top left
    position = origin;
    ndc(position);
    gl_Position = vec4(position, 1.0, 1.0);
    fragment.color = color;
    fragment.tex_coord = vec2(u1, v1);
    EmitVertex();

    // bottom left
    position = origin;
    position.y += u_glyph_size.y;
    ndc(position);
    gl_Position = vec4(position, 1.0, 1.0);
    fragment.color = color;
    fragment.tex_coord = vec2(u1, v2);
    EmitVertex();

    // top right
    position = origin;
    position.x += u_glyph_size.x;
    ndc(position);
    gl_Position = vec4(position, 1.0, 1.0);
    fragment.color = color;
    fragment.tex_coord = vec2(u2, v1);
    EmitVertex();

    // bottom right
    position = origin;
    position += u_glyph_size;
    ndc(position);
    gl_Position = vec4(position, 1.0, 1.0);
    fragment.color = color;
    fragment.tex_coord = vec2(u2, v2);
    EmitVertex();

    EndPrimitive();
}
)RAW";

    const std::string fss = R"RAW(
#version 330 core

uniform sampler2D tex;

in f_struct {
    vec3 color;
    vec2 tex_coord;
} fragment;

void main() {
    gl_FragColor = texture(tex, fragment.tex_coord) * vec4(fragment.color, 1.0);
}
)RAW";

    load_source(vss, gss, fss);

    u_view_port_ = glGetUniformLocation(program_, "u_view_port");
    u_glyph_size_ = glGetUniformLocation(program_, "u_glyph_size");
    u_tex_size_ = glGetUniformLocation(program_, "u_tex_size");

    assert(u_view_port_ > -1);
    assert(u_glyph_size_ > -1);
    assert(u_tex_size_ > -1);

    use_texture(tex);
}

void text_shader::use_glyph_size(size<uint32_t> glyph_size) {
    glyph_size_.width = static_cast<float_t> (glyph_size.width);
    glyph_size_.height = static_cast<float_t> (glyph_size.height);
}

void text_shader::use_texture(texture* tex) {
    assert(tex);
    tex_ = tex;
}

void text_shader::use_view_port(size<uint32_t> view_port) {
    view_port_.width = static_cast<float_t> (view_port.width);
    view_port_.height = static_cast<float_t> (view_port.height);
}

void text_shader::prepare() {
    glUniform2f(
        u_glyph_size_,
        glyph_size_.width,
        glyph_size_.height
    );
    glUniform2f(
        u_view_port_,
        view_port_.width,
        view_port_.height
    );

    const auto tex_size { tex_->get_size() };

    glUniform2f(
        u_tex_size_,
        static_cast<float_t> (tex_size.width),
        static_cast<float_t> (tex_size.height)
    );

    tex_->bind();
}
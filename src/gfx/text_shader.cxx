#include "text_shader.hxx"

j_text_shader::j_text_shader() {
    const std::string_view vss = R"RAW(
#version 400 core
layout (location = 0) in int glyph;
layout (location = 1) in vec4 color;

out v_struct {
    int glyph;
    vec4 color;
} vertex;

void main() {
    vertex.glyph = glyph;
    vertex.color = color;
}
)RAW";

    const std::string_view gss = R"RAW(
#version 400 core
 
layout (points) in;
layout (triangle_strip, max_vertices=4) out;

uniform uvec2 u_resolution;
uniform uvec2 u_glyph_size;
uniform uvec2 u_tex_size;

in v_struct {
    int glyph;
    vec4 color;
} vertex[];

out f_struct {
    vec4 color;
    vec2 tex_coord;
} fragment;

void ndc(inout vec2 p) { // view port to ndc coordinates
    p.x = 2.0 * p.x / u_resolution.x - 1.0;
    p.y = 1.0 - 2.0 * p.y / u_resolution.y;
}

void main() {
    // runs once for each vertex given since we pass in points
    vec4 color = vertex[0].color;
    int glyph = vertex[0].glyph;
    int points_processed = gl_PrimitiveIDIn;
    int chars_per_line = int(u_resolution.x / u_glyph_size.x);
    int chars_per_row_in_texture = int(u_tex_size.x / u_glyph_size.x);

    // pixel origin in texture
    vec2 tex_origin = vec2(
        glyph % chars_per_row_in_texture * u_glyph_size.x,
        floor(glyph / chars_per_row_in_texture) * u_glyph_size.y
    );

    // calculate tex coords for the letter box
    float u1 = 1.0 / u_tex_size.x * tex_origin.x;
    float u2 = 1.0 / u_tex_size.x * (tex_origin.x + u_glyph_size.x);
    float v1 = 1.0 / u_tex_size.y * tex_origin.y;
    float v2 = 1.0 / u_tex_size.y * (tex_origin.y + u_glyph_size.y);

    // pixel origin in 2d plane (reused across vertices)
    vec2 origin = vec2(
        // not sure why, but using `mod` causes _very weird_ behavior
        points_processed % chars_per_line * u_glyph_size.x,
        int(points_processed / chars_per_line) * u_glyph_size.y
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

    const std::string_view fss = R"RAW(
#version 400 core

uniform sampler2D tex;

in f_struct {
    vec4 color;
    vec2 tex_coord;
} fragment;

void main() {
    gl_FragColor = texture(tex, fragment.tex_coord) * vec4(fragment.color);
}
)RAW";
    if (!(compile(j_shader_type::vertex, vss)
        && compile(j_shader_type::geometry, gss)
        && compile(j_shader_type::fragment, fss)
        && link())) {
        LOG(ERROR) << "Text shader creation failed";
        return;
    }

    u_resolution_ = glGetUniformLocation(program_, "u_resolution");
    u_glyph_size_ = glGetUniformLocation(program_, "u_glyph_size");
    u_tex_size_ = glGetUniformLocation(program_, "u_tex_size");

    assert(u_resolution_ > -1);
    assert(u_glyph_size_ > -1);
    assert(u_tex_size_ > -1);
}

void j_text_shader::use_glyph_size(j_size<uint32_t> glyph_size) {
    glyph_size_ = glyph_size;
}

void j_text_shader::use_texture(j_texture&& tex) {
    assert(tex.is_loaded());
    tex_ = std::move(tex);
}

void j_text_shader::use_resolution(j_size<uint32_t> resolution) {
    resolution_ = resolution;
}

void j_text_shader::prepare() {
    glUniform2ui(u_glyph_size_, glyph_size_.width, glyph_size_.height);
    glUniform2ui(u_resolution_, resolution_.width, resolution_.height);

    const auto tex_size { tex_.size() };
    glUniform2ui(u_tex_size_, tex_size.width, tex_size.height);

    tex_.bind();
}

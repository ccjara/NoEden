#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(location = 0) uniform ivec2 u_resolution;
layout(location = 1) uniform ivec2 u_glyph_size;
layout(location = 2) uniform ivec2 u_tex_size;

layout(location = 0) flat in int in_glyph[];
layout(location = 1) in vec3 in_color[];

layout(location = 0) out vec3 out_color;
layout(location = 1) out vec2 out_tex_coord;

void ndc(inout vec2 p) {  // Viewport to NDC coordinates
    p.x = 2.0 * p.x / u_resolution.x - 1.0;
    p.y = 1.0 - 2.0 * p.y / u_resolution.y;
}

void main() {
    vec3 color = in_color[0];
    int glyph = in_glyph[0];

    if (glyph == 0) {
        return;
    }

    int points_processed = gl_PrimitiveIDIn;
    int chars_per_line = int(u_resolution.x / u_glyph_size.x);
    int chars_per_row_in_texture = int(u_tex_size.x / u_glyph_size.x);

    // Pixel origin in texture
    vec2 tex_origin = vec2(
        (glyph % chars_per_row_in_texture) * u_glyph_size.x,
        floor(glyph / chars_per_row_in_texture) * u_glyph_size.y
    );

    // Calculate tex coords for the letter box
    float u1 = 1.0 / u_tex_size.x * tex_origin.x;
    float u2 = 1.0 / u_tex_size.x * (tex_origin.x + u_glyph_size.x);
    float v1 = 1.0 / u_tex_size.y * tex_origin.y;
    float v2 = 1.0 / u_tex_size.y * (tex_origin.y + u_glyph_size.y);

    // Pixel origin in 2D plane (reused across vertices)
    vec2 origin = vec2(
        points_processed % chars_per_line * u_glyph_size.x,
        int(points_processed / chars_per_line) * u_glyph_size.y
    );

    vec2 position;

    // Top left
    position = origin;
    ndc(position);
    gl_Position = vec4(position, 1.0, 1.0);
    out_color = color;
    out_tex_coord = vec2(u1, v1);
    EmitVertex();

    // Bottom left
    position = origin;
    position.y += u_glyph_size.y;
    ndc(position);
    gl_Position = vec4(position, 1.0, 1.0);
    out_color = color;
    out_tex_coord = vec2(u1, v2);
    EmitVertex();

    // Top right
    position = origin;
    position.x += u_glyph_size.x;
    ndc(position);
    gl_Position = vec4(position, 1.0, 1.0);
    out_color = color;
    out_tex_coord = vec2(u2, v1);
    EmitVertex();

    // Bottom right
    position = origin;
    position += u_glyph_size;
    ndc(position);
    gl_Position = vec4(position, 1.0, 1.0);
    out_color = color;
    out_tex_coord = vec2(u2, v2);
    EmitVertex();

    EndPrimitive();
}

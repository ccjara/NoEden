#version 450

layout(location = 0) in int glyph;
layout(location = 1) in vec3 color;

layout(location = 0) flat out int out_glyph;
layout(location = 1) out vec3 out_color;

void main() {
    out_glyph = glyph;
    out_color = color;
}

#include "text_shader.hxx"

void text_shader::load() {
    const std::string vss = R"RAW(
#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_col;

out vec3 col;

void main() {
    gl_Position = vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);
    col = in_col; // passthrough
}
)RAW";

    const std::string fss = R"RAW(
#version 330 core
out vec4 out_col;
in vec3 col;

void main() {
    out_col = vec4(col, 1.0);
}
)RAW";

    load_source(vss, fss);
}

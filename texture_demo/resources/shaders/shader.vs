#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex_coords;
out vec3 v_color;
out vec2 v_tex_coords;

uniform mat4 modelview;
uniform mat4 projection;

void main() {
    gl_Position = projection * (modelview * vec4(position, 1.));
    v_color = color;
    v_tex_coords = tex_coords;
}

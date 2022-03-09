#version 330

in vec3 position;
in vec3 color;
in vec2 tex_coords;
out vec3 v_color;
out vec2 v_tex_coords;

uniform mat4 modelview;
uniform mat4 projection;

void main() {
    gl_Position = projection * (modelview * vec4(position, 1.));
    v_color = color;
    v_tex_coords = tex_coords;
}

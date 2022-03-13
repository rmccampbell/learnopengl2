#version 330

in vec3 v_color;
in vec2 v_tex_coords;
out vec4 frag_color;

uniform sampler2D tex;

void main() {
    frag_color = vec4(v_color, 1) * texture(tex, v_tex_coords);
}

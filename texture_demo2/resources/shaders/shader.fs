#version 330

in vec2 v_tex_coords;
out vec4 frag_color;

uniform sampler2D tex;

void main() {
    frag_color = texture(tex, v_tex_coords);
}

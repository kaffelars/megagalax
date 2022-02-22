#version 420

precision highp float;

layout (location = 0) out vec4 frag_rgb;

in vec4 color;

void main(void) {
    vec4 colour = color;

    frag_rgb = colour;
}

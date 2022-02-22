#version 420

precision highp float;

layout (location = 0) out vec4 frag_rgb;

in float mass;

void main(void) {
    vec4 colour = vec4(1.0, 1.0, 1.0, 1.0);

    frag_rgb = colour;
}

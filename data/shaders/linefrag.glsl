#version 420

precision highp float;


in vec3 rgb;

out vec4 fragcolor;

uniform float opacity;



void main(void) {
    fragcolor = vec4(rgb, opacity);
}

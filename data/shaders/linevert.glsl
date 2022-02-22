#version 330
layout(location=0) in vec4 in_div;

uniform mat4 pv;

out vec3 rgb;

void main(void) {
    vec3 f = vec3(1.0, 256.0, 65536.0);
    vec4 point = vec4(in_div.xyz, 1.0);

    rgb = fract(f * in_div.a);

	gl_Position = pv*point;
}

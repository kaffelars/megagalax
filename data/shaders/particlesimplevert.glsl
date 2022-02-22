#version 330
layout(location=0) in vec4 in_pos;

uniform mat4 pv;
uniform float particlesize;

out float mass;


void main(void) {
	vec4 vertexpos = vec4(in_pos.xyz, 1.0);

    vec4 poto = pv*vertexpos;
	gl_Position = poto;

	mass = in_pos.a;

	gl_PointSize = particlesize;
}

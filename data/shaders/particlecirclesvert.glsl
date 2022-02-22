#version 330
layout(location=0) in vec4 in_pos;

uniform mat4 pv;
uniform vec3 campos;
uniform float circlesize;
uniform float massdependence;

out float mass;


void main(void) {
	vec4 vertexpos = vec4(in_pos.xyz, 1.0);

    vec4 poto = pv*vertexpos;
	gl_Position = poto;

	mass = in_pos.a;

	float sizemass = mass;

	if (mass > 990)
    {
        sizemass = sizemass / 900.0;
    }

	float mdep = (sizemass) * massdependence;

	gl_PointSize = (circlesize + mdep) / poto.z;
}

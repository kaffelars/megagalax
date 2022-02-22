#version 330
layout(location=0) in vec4 in_pos;

uniform mat4 pv;
uniform vec3 campos;
uniform float starsize;

out float mass;


void main(void) {
	vec4 vertexpos = vec4(in_pos.xyz, 1.0);

    vec4 poto = pv*vertexpos;
	gl_Position = poto;

	mass = in_pos.a;

	float particlesize = starsize;

	if (mass > 990.0)
    {
        particlesize = starsize + sqrt((mass - 990.0) / 32.0);
    }
    else if (mass > 500.0)
    {
        particlesize = starsize + (mass - 500.0) / 32.0;
    }

	gl_PointSize = particlesize / poto.z;
}

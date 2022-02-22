#version 330
layout(location=0) in vec4 in_pos;

uniform mat4 pv;

out vec4 color;

void main(void) {
	vec4 vertexpos = vec4(in_pos.xyz, 1.0);

    vec4 poto = pv*vertexpos;
	gl_Position = poto;

	float sizos;

	if (in_pos.a <= 990)
    {
        sizos = in_pos.a / 100.0;
        color = vec4(1.0, 1.0, 1.0, 0.6);
    }
    else
    {
        sizos = clamp(sqrt(in_pos.a) / 50.0, 3.0f, 900.0f);
        color = vec4(0.7, 0.7, 1.0, 1.0);
    }

	gl_PointSize = sizos;
}

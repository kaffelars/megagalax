#version 430

layout(std140, binding = 0) buffer ptpos
{
    vec4 pos[50000];
};

layout(std140, binding = 1) buffer ptvel
{
    vec4 vel[50000];
};

layout(std140, binding = 2) buffer tempvel
{
    vec4 tvel[50000];
};

uniform float slowdown;

layout(local_size_x = 512, local_size_y = 1, local_size_z = 1) in;


void main(void)
{
    uint totid = gl_GlobalInvocationID.x;

    vec3 curpos = pos[totid].xyz;
    vec3 curvel = tvel[totid].xyz;


    curpos = curpos + curvel / slowdown; //slowmo

    pos[totid].a = pos[totid].a + tvel[totid].a;

    pos[totid].xyz = curpos;
    vel[totid].xyz = curvel;
}

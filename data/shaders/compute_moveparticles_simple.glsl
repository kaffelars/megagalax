#version 430

layout(std140, binding = 0) buffer ptpos
{
    vec4 pos[<c_maxparticles>];
};

layout(std140, binding = 1) buffer ptvel
{
    vec4 vel[<c_maxparticles>];
};


uniform float slowdown;

layout(local_size_x = 512, local_size_y = 1, local_size_z = 1) in;


void main(void)
{
    uint totid = gl_GlobalInvocationID.x;

    vec3 curpos = pos[totid].xyz;
    vec3 curvel = vel[totid].xyz;

    curpos = curpos + curvel / slowdown; //slowmo

    pos[totid].xyz = curpos;
}

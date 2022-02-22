#version 450

layout(std140, binding = 0) buffer ptpos
{
    vec4 pos[50000];
};

layout(std140, binding = 1) buffer ptvel
{
    vec4 vel[50000];
};

uniform int particlenumber;
uniform float gravity;

layout(local_size_x = 512, local_size_y = 1, local_size_z = 1) in;

void main(void)
{
    uint totid = gl_GlobalInvocationID.x;

    vec3 curpos = pos[totid].xyz;
    vec3 curvel = vel[totid].xyz;

    float gconst = gravity;


    for (int a = 0; a < particlenumber; a++)
    {
        if (a != totid)
        {
            vec3 point_separation = curpos - pos[a].xyz;
            float mass = pos[a].a;
            float disto = length(point_separation) * 2.0;
            if (disto < 0.0001f) disto = 0.0001f;//to avoid extreme bouncy bounce

            if (disto > 0.0f)
            {
                float gravo = gconst * mass / (disto * disto); //grav

                curvel = curvel - point_separation * gravo;

            }
        }
    }

    vel[totid].xyz = curvel;
}

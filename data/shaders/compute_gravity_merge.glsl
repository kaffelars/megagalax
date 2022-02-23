#version 450

layout(std140, binding = 0) buffer ptpos
{
    vec4 pos[<c_maxparticles>];
};

layout(std140, binding = 1) buffer ptvel
{
    vec4 vel[<c_maxparticles>];
};


uniform int particlenumber;
uniform float gravity;
uniform float interactiondistance;

layout(local_size_x = <shaderworkgroups>, local_size_y = 1, local_size_z = 1) in;

void main(void)
{
    uint totid = gl_GlobalInvocationID.x;

    vec3 curpos = pos[totid].xyz;
    vec3 curvel = vel[totid].xyz;
    float curmass = pos[totid].a;

    float gconst = gravity;

    float mergewith = -2.0;

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

                if (disto < interactiondistance)
                {
                    //merge
                    mergewith = a;
                }
            }
        }
    }

    vel[totid].xyz = curvel;
    vel[totid].a = mergewith;
}

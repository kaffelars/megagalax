#version 450

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

uniform int particlenumber;
uniform float gravity;
uniform float veltransfer;
uniform float masstransfer;
uniform float interactiondistance;

layout(local_size_x = 512, local_size_y = 1, local_size_z = 1) in;

void main(void)
{
    uint totid = gl_GlobalInvocationID.x;

    vec3 curpos = pos[totid].xyz;
    vec3 curvel = vel[totid].xyz;
    float curmass = pos[totid].a;

    float gconst = gravity;

    float masschange = 0;

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
                    float dfactor = (interactiondistance - disto) / interactiondistance;

                    if (veltransfer > 0.0)
                    {
                        float effect = veltransfer * dfactor * clamp((mass - curmass) / 1000.0 , 0.0, 1.0);

                        curvel = vel[a].xyz * (effect) + curvel * (1.0 - effect);
                    }

                    if (masstransfer > 0.0)
                    {
                        float diffo = curmass - mass;
                        if (diffo < 0.0) //other particle larger - lose mass
                        {
                            masschange = masschange - (curmass / 5.0) * masstransfer * dfactor;
                        }
                        else if (diffo > 0.0) //other particle smaller - gain 10% of its mass
                        {
                            masschange = masschange + (mass / 5.0) * masstransfer * dfactor;
                        }
                    }
                }
            }
        }
    }

    tvel[totid].xyz = curvel;
    tvel[totid].a = masschange;
}

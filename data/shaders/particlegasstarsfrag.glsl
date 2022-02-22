#version 420

precision highp float;

layout (location = 0) out vec4 frag_rgb;

uniform float opacity;
uniform float starmass;

in float mass;

void main(void) {
    vec4 colour;

    vec2 fragpos = gl_PointCoord - vec2(0.5, 0.5);

    float dist = length(fragpos) * 2.0;

    float stardelta = (990.0f - starmass) / 5.0;

    if (mass > 990.0) //supermassive
    {
        float blueness = clamp((sqrt(mass)/300.0) - 0.1, 0.0, 0.3);
        colour = vec4(0.9, 0.9, 0.9 + blueness, 1.0);
        if (dist < 0.8) dist = 0.0;
        colour.a = clamp((1.0 - dist), 0.0, 1.0);
    }
    else if (mass > starmass) //stars
    {
        if (mass < starmass + stardelta)
            colour = vec4(0.8, 0.2, 0.0, 1.0);
        else if (mass < starmass + stardelta*2)
            colour = vec4(1.0, 0.6, 0.2, 1.0);
        else if (mass < starmass + stardelta*3)
            colour = vec4(1.0, 1.0, 0.4, 1.0);
        else if (mass < starmass + stardelta*4)
            colour = vec4(1.0, 1.0, 1.0, 1.0);
        else
            colour = vec4(0.7, 0.7, 1.0, 1.0);

        if (dist < 0.55) colour.a = 1.0;
        else
        colour.a = clamp(1.0 - ((dist - 0.55) / 0.45), 0.0, 1.0);
    }
    else
    {
        float brightness = clamp((sqrt(mass/12.0)/6.0), 0.1, 0.9);
        colour = vec4(brightness, brightness, brightness, 1.0);
        colour.a = clamp((1.0 - dist)*opacity, 0.0, 1.0);
    }


    frag_rgb = colour;
}

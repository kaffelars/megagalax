#version 420

precision highp float;

layout (location = 0) out vec4 frag_rgb;

uniform float opacity;

in float mass;

void main(void) {
    vec4 colour;

    vec2 fragpos = gl_PointCoord - vec2(0.5, 0.5);

    float dist = length(fragpos) * 2.0;

    if (mass <= 990)
    {
        float brightness = clamp((sqrt(mass/12.0)/6.0), 0.1, 0.9);
        colour = vec4(brightness, brightness, brightness, 1.0);
        colour.a = clamp((1.0 - dist)*opacity, 0.0, 1.0);
    }
    else
    {
        float blueness = clamp((sqrt(mass)/300.0) - 0.1, 0.0, 0.3);
        colour = vec4(0.9, 0.9, 0.9 + blueness, 1.0);
        if (dist < 0.8) dist = 0.0;
        colour.a = clamp((1.0 - dist), 0.0, 1.0);
    }




    frag_rgb = colour;
}

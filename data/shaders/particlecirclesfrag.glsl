#version 420

precision highp float;

layout (location = 0) out vec4 frag_rgb;

uniform float opacity;
uniform float masscolor;

in float mass;

void main(void) {
    vec4 colour = vec4(1.0, 1.0, 0.5, 1.0);

    float massdiv = mass / 8.3;

    vec3 mc = vec3(mod(massdiv, 3.123), mod(massdiv, 4.1801), mod(massdiv, 5.717));

    mc = clamp(mc, 0.0, 1.0);

    colour.rgb = colour.rgb * (1.0 - masscolor) + mc * masscolor;

    vec2 fragpos = gl_PointCoord - vec2(0.5, 0.5);

    float dist = length(fragpos) * 2.0;

    if (dist > 0.95)
    {
        colour.a = clamp((1.0 - dist) / 0.05, 0.0, 1.0);
    }

    colour.a = colour.a * opacity;

    frag_rgb = colour;
}

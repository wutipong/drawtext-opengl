#version 460 core
in vec2 TexCoord;
out vec4 color;

uniform sampler2D msdf;
uniform float pxRange;
uniform vec4 fgColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec2 msdfUnit = pxRange/TexCoord;
    vec3 px = texture(msdf, TexCoord).rgb;
    float sigDist = median(px.r, px.g, px.b) - 0.5;
    sigDist *= dot(msdfUnit, vec2(0.5, 0.5));
    float opacity = clamp(sigDist + 0.5, 0.0, 1.0);

    color = fgColor;
    color.a = color.a * opacity;
}
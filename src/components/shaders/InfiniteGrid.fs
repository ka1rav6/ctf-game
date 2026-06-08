#version 330 core

in vec3 nearPoint;
in vec3 farPoint;

out vec4 fragColor;

uniform mat4 view;
uniform mat4 projection;
uniform float nearPlane;
uniform float farPlane;

float computeLinearDepth(vec3 pos)
{
    vec4 clip = projection * view * vec4(pos, 1.0);
    float ndc = clip.z / clip.w;

    float linear = (2.0 * nearPlane * farPlane) /
                   (farPlane + nearPlane - ndc * (farPlane - nearPlane));

    return linear / farPlane;
}

vec4 grid(vec3 worldPos, float scale)
{
    vec2 coord = worldPos.xz * scale;

    vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    float line = min(grid.x, grid.y);

    float alpha = 1.0 - min(line, 1.0);

    vec4 color = vec4(0.2, 0.2, 0.2, alpha);

    // axis highlight
    if (abs(worldPos.x) < 0.02) color = vec4(0.8, 0.1, 0.1, 1.0);
    if (abs(worldPos.z) < 0.02) color = vec4(0.1, 0.1, 0.8, 1.0);

    return color;
}

void main()
{
    float denom = (farPoint.y - nearPoint.y);

    if (abs(denom) < 0.00001)
        discard;

    float t = -nearPoint.y / denom;

    if (t < 0.0)
        discard;

    vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

    float fade = 1.0 - clamp(length(worldPos) * 0.001, 0.0, 1.0);

    vec4 g1 = grid(worldPos, 1.0);
    vec4 g2 = grid(worldPos, 0.1);

    fragColor = (g1 + g2) * fade;
}
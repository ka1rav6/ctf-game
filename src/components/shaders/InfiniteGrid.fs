#version 330 core

in vec3 nearPoint;
in vec3 farPoint;

out vec4 fragColor;

float grid(vec2 coord)
{
    vec2 g = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    return 1.0 - min(g.x, g.y);
}

void main()
{
    float denom = (farPoint.y - nearPoint.y);
    if (abs(denom) < 0.0001) discard;

    float t = -nearPoint.y / denom;
    if (t < 0.0) discard;

    vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

    // 🌿 base grass color
    vec3 baseColor = vec3(0.10, 0.45, 0.15);

    // grid
    float g1 = grid(worldPos.xz * 1.0);
    float g2 = grid(worldPos.xz * 0.1);

    float g = max(g1, g2);

    // soften grid into terrain variation
    float mask = smoothstep(0.0, 1.0, g);

    vec3 color = mix(baseColor, baseColor * 0.6, mask);

    fragColor = vec4(color, 1.0);
}
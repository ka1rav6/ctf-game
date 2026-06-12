#version 330 core

// in vec3 nearPoint;
// in vec3 farPoint;
in vec3 worldLoc;

out vec4 fragColor;

float grid(vec2 coord)
{
    vec2 g = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    return 1.0 - min(g.x, g.y);
}

void main()
{
    vec3 baseColor = vec3(0.10, 0.45, 0.15);
    float g1 = grid(worldLoc.xz * 1.0);
    float g2 = grid(worldLoc.xz * 0.25);
    if (g2 > 0.6){
			fragColor = vec4(vec3(0.2), 1.0);
		}else if(g1 > 0.05){
			fragColor = vec4(vec3(0.6), 1.0);
		}else{
			// set this to ground color agr isse hi ground bnana hai
			fragColor = vec4(0.0);
			return;
		}

}


/*

/// IF SMOOTH FLOOR IS NEEDED INSTEAD (NOTE THE SHADOW IS NOT WORKING YET)
#version 330 core

in vec3 nearPoint;
in vec3 farPoint;

out vec4 fragColor;

void main()
{
    float denom = (farPoint.y - nearPoint.y);
    if (abs(denom) < 0.0001) discard;

    float t = -nearPoint.y / denom;
    if (t < 0.0) discard;

    vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

    vec3 grass = vec3(0.10, 0.55, 0.20);

    fragColor = vec4(grass, 1.0);
}
*/

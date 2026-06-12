#version 330 core

#define d 30.0

// out vec3 nearPoint;
// out vec3 farPoint;
out vec3 worldLoc;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraLoc;

vec3 gridPlane[4] = vec3[](
    vec3(-d, 0, -d),
    vec3(-d, 0,  d),
    vec3( d, 0, -d),
    vec3( d, 0,  d)
);

void main()
{
    vec3 p = gridPlane[gl_VertexID];
    vec3 pos = vec3(cameraLoc.x + p.x, 0.0, cameraLoc.z + p.z);
    worldLoc = pos;
    gl_Position = projection * view * vec4(pos, 1.0);

    // nearPoint = unproject(vec3(p.x, p.y, -1.0));
    // farPoint  = unproject(vec3(p.x, p.y,  1.0));

    // gl_Position = vec4(p.xy, 0.9999999, 1.0);
}

#version 330 core

out vec3 nearPoint;
out vec3 farPoint;

uniform mat4 view;
uniform mat4 projection;

vec3 gridPlane[4] = vec3[](
    vec3(-1,  1, 0),
    vec3(-1, -1, 0),
    vec3( 1,  1, 0),
    vec3( 1, -1, 0)
);

vec3 unproject(vec3 p)
{
    mat4 invVP = inverse(projection * view);
    vec4 world = invVP * vec4(p, 1.0);
    return world.xyz / world.w;
}

void main()
{
    vec3 p = gridPlane[gl_VertexID];

    nearPoint = unproject(vec3(p.x, p.y, -1.0));
    farPoint  = unproject(vec3(p.x, p.y,  1.0));

    gl_Position = vec4(p, 1.0);
}
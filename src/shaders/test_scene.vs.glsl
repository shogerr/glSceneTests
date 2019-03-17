#version 410 core

#define M_PI 3.1415926535897932384626433832795

layout(location = 0) in vec4 pos;
layout(location = 1) in mat4 MVP;

mat4 t = mat4(1.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 1.0);


float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

out vec4 color_out;

void main()
{
    gl_Position = MVP * pos;
    color_out = vec4(1 - abs(pos.x) + MVP[2].z, 1 - abs(pos.y), 1 - abs(pos.z), 1.0f);
}

#version 460

layout(location=0) uniform mat4 view;
layout(location=1) uniform mat4 projection;

layout(location=0) in vec3 a_vertex;

void main(void)
{
    gl_Position = projection * view * vec4(a_vertex, 1.);
    //gl_Position = projection * view * a_vertex;
}
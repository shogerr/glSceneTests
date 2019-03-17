#version 460 core

layout(location=0) uniform mat4 view;
layout(location=1) uniform mat4 projection;

layout(location=0) in vec4 a_vertex;

void main(void)
{
    gl_Position = projection * view * a_vertex;
}
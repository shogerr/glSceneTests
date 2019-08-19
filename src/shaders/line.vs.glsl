#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in mat4 model;
layout(location=5) in vec4 color;

layout(location=0) uniform mat4 view;
layout(location=1) uniform mat4 projection;

out vec4 color_;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1);
}

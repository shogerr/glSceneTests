
#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoords;
layout(location=3) in mat4 model;

layout(location=0) uniform mat4 view;
layout(location=1) uniform mat4 projection;

out vec2 texcoords_;
out vec3 color_;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1);
	texcoords_ = texcoords;
	color_ = vec3(0., 0., 0.8);
}

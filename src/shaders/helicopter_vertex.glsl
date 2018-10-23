#version 410 core

layout(location=0) in vec4 pos;
layout(location=1) in vec3 normal;
layout(location=2) in mat4 MVP;

out vec4 color_out;

void main()
{
	gl_Position = MVP * pos;
	color_out = vec4(normal.z, (normal.z+1)/2, normal.z, 1.0);
}


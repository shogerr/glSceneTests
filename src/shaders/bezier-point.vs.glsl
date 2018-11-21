#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in mat4 mvp;

layout(location=0) uniform mat4 view;
layout(location=1) uniform mat4 projection;

void main()
{
	gl_PointSize = 5.0;
	gl_Position = projection*view*mvp*vec4(pos, 1);
}


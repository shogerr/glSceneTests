#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in mat4 mvp;
layout(location=5) in vec4 color;

layout(location=0) uniform mat4 view;
layout(location=1) uniform mat4 projection;

out vec4 color_;
void main()
{
	float modifier = 0;
	gl_Position = projection * view * mvp * vec4(pos, 1);
	//color.b = pos.z;
	if (pos.z < 0)
		modifier = .5;
	color_ = vec4(color.r, color.g+modifier, color.b+modifier*abs(pos.z), 1.0);
}


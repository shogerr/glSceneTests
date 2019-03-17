#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 tex_coord;
layout(location=3) in mat4 model;

//layout(location=7) in vec3 color;

layout(location=0) uniform mat4 view;
layout(location=1) uniform mat4 projection;

//out vec3 view_pos_;
out vec2 tex_coord_;
out vec3 color_;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1);
	tex_coord_ = tex_coord;
	color_ = vec3(1.0);
}

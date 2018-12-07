
#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoords;
layout(location=3) in mat4 model;

layout(location=0) uniform mat4 view;
layout(location=1) uniform mat4 projection;
layout(location=2) uniform vec3 view_pos;

out vec2 texcoords_;
out vec3 color_;
out vec3 view_pos_;
out vec3 frag_pos_;
out vec3 normal_;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1);
	texcoords_ = texcoords;
	color_ = vec3(0., 0., 0.8);
	frag_pos_ = vec3(model * vec4(pos, 1.0));
	view_pos_ = view_pos;
	normal_ = mat3(transpose(inverse(model))) * normal;
}

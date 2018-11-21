#version 460 core

#define M_PI 3.1415926535897932384626433832795

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 tex_coord;
layout(location=3) in mat4 model_;

layout(location=0) uniform mat4 view;
layout(location=1) uniform mat4 projection;
layout(location=2) uniform vec3 view_pos;

out vec3 view_pos_;
out vec2 tex_coord_;
out vec3 frag_pos_;
out vec3 normal_;

mat4 model = mat4(1.0);

void main()
{
	gl_Position = projection * view * model_ * vec4(pos, 1.0);

	frag_pos_ = vec3(model_ * vec4(pos, 1.0));
	normal_ = mat3(transpose(inverse(model_))) * normal;
	tex_coord_ = tex_coord;
}


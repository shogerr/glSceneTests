#version 460 core

out vec4 color;

//in vec2 tex_coord_;
in vec3 color_;

layout(location=4) uniform sampler2D texture_diffuse;
layout(location=5) uniform sampler2D texture_specular;

void main()
{
	color = vec4(1.0);
	//color = texture(texture_diffuse, tex_coord_);
}

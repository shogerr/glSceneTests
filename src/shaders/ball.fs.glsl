
#version 460 core

out vec4 color;

in vec2 texcoords_;
in vec3 color_;

layout(location=4) uniform sampler2D texture_diffuse;
layout(location=5) uniform sampler2D texture_specular;

void main()
{
	color = texture(texture_diffuse, texcoords_);
}

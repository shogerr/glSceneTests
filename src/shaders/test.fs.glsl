#version 140
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

out vec4 color;

in vec2 texcoords_;
in vec3 color_;

layout(location=4) uniform sampler2D texture_diffuse;
layout(location=5) uniform sampler2D texture_specular;

void main()
{
	//color = vec4(1.0);
	color = texture(texture_diffuse, texcoords_);
}

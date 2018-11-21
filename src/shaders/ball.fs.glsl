
#version 460 core

out vec4 color;

in vec2 texcoords_;
in vec3 color_;

layout(location=4) uniform sampler2D texture_diffuse;

void main()
{
	color = texture(texture_diffuse, texcoords_);
}

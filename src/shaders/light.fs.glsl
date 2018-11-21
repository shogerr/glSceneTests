#version 460 core

out vec4 color;

in vec2 tex_coord_;
in vec3 color_;

layout(location=4) uniform sampler2D texture_diffuse;

void main()
{
	color = vec4(color_, 1.0);
	//color = texture(texture_diffuse, tex_coord_);
}

#version 410 core

in vec4 color_out_;
in vec2 tex_coord_;

out vec4 color;

uniform sampler2D texture_;

void main()
{
	color = texture(texture_, tex_coord_);
}

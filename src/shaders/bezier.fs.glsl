#version 460 core

out vec4 color;

in vec4 color_;

void main()
{
	color = color_;
	//color = vec4(1.0, 0.0, 0.0, 1.0);
}

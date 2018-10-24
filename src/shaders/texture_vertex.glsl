#version 410 core

#define M_PI 3.1415926535897932384626433832795

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 tex_coord;

out vec2 tex_coord_;
out vec4 color_out_;

uniform float dt;
uniform vec2 focus;
uniform int toggle_animation;

mat4 scale = mat4(2.0);
mat4 rotate = mat4(1.0);
mat4 rotate_y = mat4(1.0);
mat4 translate = mat4(1.0);

void main()
{
	float theta = -M_PI/2.5;
	float theta_2 = M_PI/6;

	scale[3].w = 1.0;
	rotate[1].y = cos(theta);
	rotate[1].z = sin(theta);
	rotate[2].y = -sin(theta);
	rotate[2].z = cos(theta);

	rotate_y[0].x = cos(theta_2);
	rotate_y[0].z = -sin(theta_2);
	rotate_y[2].x = sin(theta_2);
	rotate_y[2].z = cos(theta_2);

	translate[3].y = -.1;
	translate[3].x = .2;
	translate[3].z = .35;

	gl_Position = translate * rotate_y * rotate * scale * vec4(pos, 1.0);
	float r = sqrt(pow(tex_coord.x - focus.x, 2) + pow(tex_coord.y - focus.y, 2));

	float rn = pow(r, dt);

	//tex_coord_.x = rn * (tex_coord.x - .5)/r;
	//tex_coord_.y = rn * (tex_coord.y - .5)/r;
	tex_coord_.x = rn * cos(atan(tex_coord.x - focus.x, tex_coord.y - focus.y)) + focus.x;
	tex_coord_.y = rn * sin(atan(tex_coord.x - focus.x, tex_coord.y - focus.y)) + focus.y;

	if (toggle_animation == 3)
	{
		tex_coord_.x = tex_coord.x;
		tex_coord_.y = tex_coord.y;
	}

	color_out_ = vec4(normal.z, (normal.z+1)/2, normal.z, 1.0);
}


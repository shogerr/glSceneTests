#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoords;
layout(location=3) in mat4 model;

layout(location=0) uniform mat4 view;
layout(location=1) uniform mat4 projection;
layout(location=2) uniform vec3 view_pos;
layout(location=3) uniform vec2 time;

out vec3 color_;
out vec2 texcoords_;
out vec3 view_pos_;
out vec3 normal_;
out vec3 frag_pos_;
out float time_;

vec4 DoTwist( vec4 pos, float t )
{
	float st = sin(t);
	float ct = cos(t);
	vec4 new_pos = pos;
	
	// Perform a rotation of the x and z coordinates
	new_pos.x = pos.x*ct - pos.z*st;
	new_pos.z = pos.x*st + pos.z*ct;
	
	return( new_pos );
}

void main()
{
	float height = 1.0;
	float angle_deg = 180.0*sin(time.x);
	float angle_rad = angle_deg * 3.14159 / 180.0;
	
	float ang = (height*0.5 + pos.y)/height * angle_rad;
	vec4 twisted_normal = DoTwist(vec4(normal, 1.0), ang);
	gl_Position = projection * view * model * DoTwist(vec4(pos, 1.0), ang);

	frag_pos_ = vec3(model * vec4(pos, 1.0));
	normal_ = vec3(transpose(inverse(model)) * twisted_normal);
	texcoords_ = texcoords;
	time_ = time.y;
}

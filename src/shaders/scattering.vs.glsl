#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in mat4 model;

//layout(location=7) in vec3 color;

layout(location = 0) uniform mat4 view;
layout(location = 1) uniform mat4 projection;
layout(location = 2) uniform mat4 depth_pass_matrix;

//out vec3 view_pos_;
out vec2 tex_coord_;
out vec3 color_;
out vec3 normal_;
out vec4 projection_coord_;

void main()
{
    vec4 vertex = model * vec4(pos, 1.);

    projection_coord_ = depth_pass_matrix * vertex;

    vertex = view * vertex;

    //normal_ = vec3(model * view * vec4(normal, 1.)).xyz;
    normal_ = mat3(projection) * mat3(view)  * mat3(model) * normal;

    gl_Position = projection * vertex;
}

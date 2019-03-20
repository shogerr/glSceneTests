#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

layout(location = 0) uniform mat4 view;
layout(location = 1) uniform mat4 projection;
layout(location = 2) uniform mat4 model;

layout(location = 3) uniform mat4 depth_pass_matrix;

layout(location = 4) uniform mat3 u_normal_matrix;


out vec3 normal_;
out vec4 projection_coord_;

void main()
{
    //vec4 vertex = model * pos;
    vec4 vertex = model * vec4(pos, 1.);

    projection_coord_ = depth_pass_matrix * vertex;

    vertex = view * vertex;

    //normal_ = vec3(model * view * vec4(normal, 1.)).xyz;

    // Sort of works.
    normal_ = mat3(projection) * mat3(view)  * mat3(model) * normal;
    
    // Intended?
    //normal_ = mat3(view * model) * normal;
    normal_ = u_normal_matrix * normal;

    gl_Position = projection * vertex;
}

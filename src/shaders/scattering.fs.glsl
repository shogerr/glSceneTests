#version 460

out vec4 frag_color;

in vec4 projection_coord_;
in vec3 normal_;

layout(location = 5) uniform vec3 u_light_direction;
layout(location = 6) uniform sampler2D texture_diffuse;
layout(location = 7) uniform float u_wrap;
layout(location = 8) uniform float u_width;
layout(location = 9) uniform float u_falloff;

float WrapLighting(float x, float wrap)
{
    return (x + wrap) / (1.0 + wrap);
}

float ScatterIntensity(float x, float width)
{
    if (width == 0.0)
        return 0.0;

    return smoothstep(0.0, width, x) * smoothstep(width * 2.0, width, x);
}

float DepthPassSpace(float coord)
{
    float near = 1.0;
    float far = 100.0;

    float NDC = (coord - 0.5) * 2.0;
    return -2 * far * near / (NDC * (near - far) + far + near);
}

void main()
{
    //float u_wrap = 0.5;
    //float u_width = 0.5;
    //float u_falloff = 2.0;

    //color = vec4(1.0);
    float coord = texture(texture_diffuse, projection_coord_.xy / projection_coord_.w).r;
    //float coord = .25;
    float coord_out = projection_coord_.z / projection_coord_.w;


    float falloff = exp(-abs(DepthPassSpace(coord_out) - DepthPassSpace(coord)) * u_falloff);

    vec3 normal = normalize(normal_);

    float NdotL = dot(normal, u_light_direction);
    // Use u_wrap;
    float wrapped = WrapLighting(NdotL, u_wrap);

    float diffuse_intensity = max(NdotL, 0.0);
    // Use u_width;
    float scatter_intensity = ScatterIntensity(wrapped, u_width);

    //frag_color = texture(texture_diffuse, projection_coord_.xy/projection_coord_.w);

    //frag_color = texture(texture_diffuse, tex_coord_);
    frag_color = vec4(0.2, 1.0, 0.3, 1.0) * diffuse_intensity + vec4(1., .1, 0.1, 1.0) * scatter_intensity * falloff;
    //frag_color = vec4(1., .5, .5, 1.);
}

#version 460 core
out vec4 color;

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define POINT_LIGHT_COUNT 2

in vec3 view_pos_;
in vec3 frag_pos_;
in vec3 normal_;
in vec2 tex_coord_;

layout(location=4) uniform sampler2D texture_diffuse;
layout(location=5) uniform sampler2D texture_specular;

layout(location=6) uniform PointLight point_lights[POINT_LIGHT_COUNT];
layout(location=20) uniform SpotLight spot_light;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    float spec = pow(max(0.0, dot(normal, normalize(lightDir + viewDir))), 128.0) * 32.0 * sqrt(diff);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse, tex_coord_));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse, tex_coord_));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular, tex_coord_));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 h = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    float spec = pow(max(0.0, dot(normal, h)), 128.0) * 32.0 * sqrt(diff);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse, tex_coord_));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse, tex_coord_));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular, tex_coord_));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
	/*
	float ambient = .15;
	vec3 view_dir = normalize(view_pos_ - frag_pos_);
	vec3 light_pos_ = vec3(0.5, 0.5, 0.25);

	vec3 light_dir = normalize(light_pos_ - frag_pos_);
	vec3 diffuse = max(dot(normal_, light_dir), 0.0) * vec3(1.0, 1.0, 1.0); 

	vec3 result = (diffuse + ambient) * vec3(1.0, 0.0, 0.0);
	*/

	vec3 view_dir = normalize(view_pos_ - frag_pos_);
	vec3 norm = normalize(normal_);

	vec3 result = CalcPointLight(point_lights[0], norm, frag_pos_, view_dir);
	result += CalcPointLight(point_lights[1], norm, frag_pos_, view_dir);
	result += CalcSpotLight(spot_light, norm, frag_pos_, view_dir);

	color = vec4(result, 1.0);
}

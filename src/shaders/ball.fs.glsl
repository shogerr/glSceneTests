#version 460 core

out vec4 color;

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

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


layout(location=4) uniform sampler2D texture_diffuse;
layout(location=5) uniform sampler2D texture_specular;
layout(location=6) uniform SpotLight spot_light;
layout(location=16) uniform PointLight point_lights[2];

in vec2 texcoords_;
in vec3 color_;
in vec3 view_pos_;
in vec3 frag_pos_;
in vec3 normal_;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 h = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    //float spec = pow(max(0.0, dot(normal, h)), 128.0) * 32.0 * sqrt(diff);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse, texcoords_));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse, texcoords_));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular, texcoords_));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    //float spec = pow(max(0.0, dot(normal, normalize(lightDir + viewDir))), 128.0) * 32.0 * sqrt(diff);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse, texcoords_));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse, texcoords_));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular, texcoords_));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
	vec3 view_dir = normalize(view_pos_ - frag_pos_);
	vec3 norm = normalize(normal_);
	vec3 result = CalcSpotLight(spot_light, norm, frag_pos_, view_dir);
	result += CalcPointLight(point_lights[0], norm, frag_pos_, view_dir);
	result += CalcPointLight(point_lights[1], norm, frag_pos_, view_dir);
	result += vec3(texture(texture_diffuse, texcoords_)) * .25;

	color = vec4(result, 1.0);
}

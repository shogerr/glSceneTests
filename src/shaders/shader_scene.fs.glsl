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


layout(location=4) uniform sampler2D texture_diffuse;
layout(location=5) uniform sampler2D texture_specular;

layout(location=6) uniform SpotLight spot_light;

in vec3 color_;
in vec2 texcoords_;
in vec3 view_pos_;
in vec3 frag_pos_;
in vec3 normal_;
in float time_;

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
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse, texcoords_));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse, texcoords_));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular, texcoords_));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

float circleSize=1.0/(3.0*pow(2.0,float(4)));

vec2 rot(vec2 uv,float a){
	return vec2(uv.x*cos(a)-uv.y*sin(a),uv.y*cos(a)+uv.x*sin(a));
}

void main()
{
	vec3 view_dir = normalize(view_pos_ - frag_pos_);
	vec3 norm = normalize(normal_);
	vec3 result = CalcSpotLight(spot_light, norm, frag_pos_, view_dir);
	result += vec3(texture(texture_diffuse, texcoords_) * .5);

	vec2 uv=vec2(1,1);
	uv=-.5*(uv-2.0*texcoords_.xy)/uv.x;
	
	float s=0.25;
	for(int i=0;i<4;i++){
		uv=abs(uv)-s;
		uv=rot(uv, time_);
		s=s/2.1;
	}
	
	//draw a circle
	float c = length(uv) > circleSize ? 0.0 : 1.0;
	//color = vec4(c,c,c,1.0);
	//c -= (sin(time_/2)+1)/2;
	color = vec4(result.x+c, result.y+(c*(sin(time_/4)+1)/2), result.z+(c*(sin(time_/2)+1)/2), 1.0);
	//color = texture(texture_diffuse, texcoords_);
	//color = vec4(.5, .5, .5, 1.0);
}

#ifndef __LIGHTING_SCENE_HPP
#define __LIGHTING_SCENE_HPP

#include <bitset>
#include "../engine_parts.hpp"

class Lighting
{
public:
    typedef struct light {
        glm::vec3 ambient = glm::vec3(.05f);
        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 specular = glm::vec3(1.0f);
    } Light;

    typedef struct pointLight {
        glm::vec3 position;

        float constant = 1.0f;
        float linear = .09f;
        float quadratic = .32f;

        Light light;
    } PointLight;

    typedef struct spotLight {
        glm::vec3 position;
        glm::vec3 direction;
        float cutoff = glm::cos(glm::radians(12.5f));
        float outer_cutoff = glm::cos(glm::radians(25.0f));

        float constant = 1.0f;
        float linear = .09f;
        float quadratic = .32f;

        Light light;
    } SpotLight;

    static void FillSpotLight(GLuint program, GLuint loc, Lighting::SpotLight* spotlight)
    {
        glProgramUniform3fv(program, loc, 1, glm::value_ptr(spotlight->position));
        glProgramUniform3fv(program, loc+1, 1, glm::value_ptr(spotlight->direction));
        glProgramUniform1f(program, loc+2, spotlight->cutoff);
        glProgramUniform1f(program, loc+3, spotlight->outer_cutoff);
        glProgramUniform1f(program, loc+4, spotlight->constant);
        glProgramUniform1f(program, loc+5, spotlight->linear);
        glProgramUniform1f(program, loc+6, spotlight->quadratic);
        glProgramUniform3fv(program, loc+7, 1, glm::value_ptr(spotlight->light.ambient));
        glProgramUniform3fv(program, loc+8, 1, glm::value_ptr(spotlight->light.diffuse));
        glProgramUniform3fv(program, loc+9, 1, glm::value_ptr(spotlight->light.specular));
    }

    static void FillPointLight(GLuint program, GLuint loc, Lighting::PointLight* pointlight)
    {
        glProgramUniform3fv(program, loc, 1, glm::value_ptr(pointlight->position));
        glProgramUniform1f(program, loc+1, pointlight->constant);
        glProgramUniform1f(program, loc+2, pointlight->linear);
        glProgramUniform1f(program, loc+3, pointlight->quadratic);
        glProgramUniform3fv(program, loc+4, 1, glm::value_ptr(pointlight->light.ambient));
        glProgramUniform3fv(program, loc+5, 1, glm::value_ptr(pointlight->light.diffuse));
        glProgramUniform3fv(program, loc+6, 1, glm::value_ptr(pointlight->light.specular));
    }
};

template<typename T>
class SceneObject
{
public:
    SceneObject()
    {
        t = new T();
        pos_ = glm::vec3(0.0);
        v_ = glm::vec3(0.0);
        theta_ = glm::vec3(0.0);
        w_ = glm::vec3(0.0);
    }

    glm::vec3 Theta()
    {
        return theta_;
    }

    void UpdateTheta(float dt)
    {
        theta_ += w_ * dt;
        for (int i = 0; i < 3; i++)
        {
            if (theta_[i] <= -glm::two_pi<float>())
                theta_[i] += glm::two_pi<float>();
            else if (theta_[i] >= glm::two_pi<float>())
                theta_[i] -= glm::two_pi<float>();
        }
    }

    void SetAngularVelocity(glm::vec3 w) { w_ = w; }

    T* t;

    glm::vec3 pos_;
    glm::vec3 v_;

    glm::vec3 theta_;
    glm::vec3 w_;
};


class LightingScene : public Scene
{
public:
    virtual void DoFrame();
    virtual void OnStartGraphics();
    
    void Poke(unsigned short e)
    {
        light_state_ ^= 1ULL << e;
    }

private:
    gl00::Model* scene_model_;
    gl00::Model* sphere_model_;
    gl00::Model* light_model_;

    GLuint color_bo_;
    glm::vec3 colors[3] = { {1.0,1.0,1.0}, {1.0,0.0,0.0}, {1.0,1.0,0.0} };

    std::vector<Shader*> shaders_;

    glm::vec3 view_position_;
    glm::mat4 projection_;
    glm::mat4 view_;
    
    uint64_t last_frame_ns_;

    std::bitset<4> light_state_;

    glm::vec3 light_pos = glm::vec3(.5,1.75,0);

    SceneObject<int> g_object;
    SceneObject<Lighting::SpotLight> g_spotlight;
    SceneObject<Lighting::PointLight> g_pointlights[2];
};


#endif // !__LIGHTING_SCENE_HPP

#ifndef __LIGHTING_SCENE_HPP
#define __LIGHTING_SCENE_HPP

#include <bitset>
#include <gl00/engine_parts.hpp>
#include "../src/gizmos/lighting.hpp"

namespace
{

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

        // Update each component's angle
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

}


class LightingScene : public gl00::Scene
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

    std::vector<gl00::Shader*> shaders_;

    glm::vec3 view_position_;
    glm::mat4 projection_;
    glm::mat4 view_;
    
    uint64_t last_frame_ns_;

    std::bitset<4> light_state_;

    glm::vec3 light_pos = glm::vec3(.5,1.75,0);

    SceneObject<int> g_object;
    SceneObject<gl00::Lighting::SpotLight> g_spotlight;
    SceneObject<gl00::Lighting::PointLight> g_pointlights[2];
};


#endif // !__LIGHTING_SCENE_HPP

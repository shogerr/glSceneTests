#ifndef __LIGHTING_SCENE_HPP
#define __LIGHTING_SCENE_HPP

#include <bitset>
#include <gl00/engine_parts.hpp>
#include "../src/gizmos/basic_scene_object.hpp"
#include "../src/gizmos/lighting.hpp"

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

    gl00::BasicSceneObject<int> g_object;
    gl00::BasicSceneObject<gl00::Lighting::SpotLight> g_spotlight;
    gl00::BasicSceneObject<gl00::Lighting::PointLight> g_pointlights[2];
};


#endif // !__LIGHTING_SCENE_HPP

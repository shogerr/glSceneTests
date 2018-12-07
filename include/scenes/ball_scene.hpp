#ifndef __BALL_SCENE_HPP
#define __BALL_SCENE_HPP

#include <engine_parts.hpp>
#include <scene_object.hpp>
#include <camera.hpp>
#include "../../src/gizmos/lighting.hpp"

class BallScene : public Scene
{
public:
    virtual void OnStartGraphics();
    virtual void DoFrame();
    virtual void Step();
    virtual void OnKillGraphics();
    virtual void OnMouseMotion(float x, float y);

    virtual void Poke(unsigned int i);

private:
    Shader* shader_;
    Shader* light_shader_;
    gl00::Camera camera_;
    glm::vec3 camera_pos_;
    glm::vec3 camera_focus_;

    gl00::Lighting::SpotLight spotlight_;
    std::vector<gl00::Lighting::PointLight> point_lights_;

    glm::vec2 cursor_pos_ = { 0.f, 0.f };

    //gl00::SceneObject<gl00::Model*>* points_;

    const float gravity_ = -9.8f;

    glm::vec3 target_;
    //std::vector<glm::vec3> obstacles_;

    gl00::Model* scene_model_;
    gl00::Model* obstacle_model_;
    gl00::Model* light_model_;

    glm::mat4* p_obstacle_model;
    glm::mat4* p_light_model_;
    
    gl00::SceneObject<gl00::Model*>* figure_;
    std::vector<gl00::StateSolver> obstacles_;

    glm::mat4 ground_plane_model_;
    gl00::Model* ground_plane_;

    uint64_t lastframe_;

    void CalcGroundHit(gl00::StateSolver &solver, float cr, float dt);
    glm::vec3 ObstacleForce();
    void SpawnObstacle();
};

#endif // !__BALL_SCENE_HPP


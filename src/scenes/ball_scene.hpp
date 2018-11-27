#ifndef __BALL_SCENE_HPP
#define __BALL_SCENE_HPP

#include "../engine_parts.hpp"
#include "../camera.hpp"

class BallScene : public Scene
{
public:
    virtual void OnStartGraphics();
    virtual void DoFrame();
    virtual void Step();
    virtual void OnMouseMotion(float x, float y);

    void Poke(int i);

private:
    Shader* shader_;
    Camera camera_;
    glm::vec3 camera_pos_ = glm::vec3(0.f, .5f, 1.5f);

    glm::vec2 cursor_pos_ = { 0, 0 };
    Model* scene_model_;

    uint64_t lastframe_;
};

#endif // !__BALL_SCENE_HPP


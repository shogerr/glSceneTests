#ifndef __BALL_SCENE_HPP
#define __BALL_SCENE_HPP

#include "../engine_parts.hpp"

class Camera
{
public:
    float aspect_ = 1.0f;
    glm::mat4 projection_ = glm::perspective(glm::radians(65.0f), aspect_, 0.1f, 100.0f);
    glm::vec3 view_position_ = glm::vec3(0.0, 1.0, -1.5);
    glm::mat4 view_ = glm::lookAt(view_position_, glm::vec3(0), glm::vec3(0, 1, 0));
};

class BallScene : public Scene
{
public:
    virtual void OnStartGraphics();
    virtual void DoFrame();

    void Poke(int i);

private:
    uint64_t lastframe_;
    Shader* shader_;
    Camera camera_;

    Model* scene_model_;
};

#endif // !__BALL_SCENE_HPP


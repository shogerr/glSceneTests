#ifndef __BALL_SCENE_HPP
#define __BALL_SCENE_HPP

#include <engine_parts.hpp>
#include <scene_object.hpp>
#include <camera.hpp>

class BallScene : public Scene
{
public:
    virtual void OnStartGraphics();
    virtual void DoFrame();
    virtual void Step();
    virtual void OnKillGraphics();
    virtual void OnMouseMotion(float x, float y);

    virtual void Poke(unsigned int i)
    { 
        LOGI("Applying force\n");
        if (i == 5)
            figure_->solver_.force_.x *= -1.0f;
        else
            figure_->solver_.force_.x = (float)i;
    }

    ~BallScene();

private:
    Shader* shader_;
    gl00::Camera camera_;
    glm::vec3 camera_pos_ = glm::vec3(0.f, .5f, 1.5f);

    glm::vec2 cursor_pos_ = { 0, 0 };

    gl00::SceneObject<gl00::Model>* figure_;

    uint64_t lastframe_;
};

#endif // !__BALL_SCENE_HPP


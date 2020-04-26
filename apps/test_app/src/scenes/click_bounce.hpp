#pragma once

#include <gl00/scene.hpp>
#include <gl00/model.hpp>
#include <gl00/camera.hpp>
#include <gl00/gizmos/scene_object.hpp>
#include <queue>

class ClickBounce : public gl00::Scene
{
public:
    ClickBounce();

    void OnStartGraphics();
    void DoFrame();
    void OnMouseMotion(float x, float y);

    unsigned int ReadMessage();
    void Poke(unsigned int value);
private:
    std::queue<uint8_t> queue_;
    gl00::Model model_;
    gl00::Model ground_;
    gl00::SceneObject<gl00::Model> scene_object_ {model_};
    gl00::Shader shader_;
    gl00::Camera camera_;
    uint64_t time_;
};
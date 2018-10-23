#include "lighting_scene.hpp"

void LightingScene::DoFrame()
{
}

void LightingScene::OnStartupGraphics()
{
    scene_model_ = new Model("../src/test/face_textured.obj");
}

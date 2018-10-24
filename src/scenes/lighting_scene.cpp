#include "lighting_scene.hpp"

void LightingScene::OnStartupGraphics()
{
    scene_model_ = new Model("../src/test/face_textured.obj");
}

void LightingScene::DoFrame()
{
    glBindVertexArray(scene_model_->meshes_[0].vao_);

    glDrawElements(GL_TRIANGLES, scene_model_->meshes_[0].indices_.size(), GL_UNSIGNED_INT, 0);
}

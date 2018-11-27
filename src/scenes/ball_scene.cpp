#include "ball_scene.hpp"
#include <chrono>

void BallScene::OnStartGraphics()
{
    std::vector<std::pair <GLenum, std::string >> shaderFilenames = { {GL_VERTEX_SHADER, "../src/shaders/ball.vs.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "../src/shaders/ball.fs.glsl"} };

    shader_ = new Shader(shaderFilenames);

    scene_model_ = new Model("../src/test/cube_object.obj", 1);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    lastframe_ = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}

void BallScene::Step()
{
    camera_.view_ = glm::lookAt(camera_pos_, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
}

void BallScene::DoFrame()
{
    uint64_t now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
    float dt = float(now - lastframe_) * 0.000000001f;
    Step();
    GLuint scene_program = shader_->program_;
    glUseProgram(scene_program);
    glProgramUniformMatrix4fv(scene_program, 0, 1, GL_FALSE, glm::value_ptr(camera_.view_));
    glProgramUniformMatrix4fv(scene_program, 1, 1, GL_FALSE, glm::value_ptr(camera_.projection_));

    glm::mat4 t = glm::translate(glm::mat4(1.0), glm::vec3(.5f,0,-.5f));
    t = glm::rotate(t, glm::radians(33.0f), glm::vec3(0,1,0));
    scene_model_->UpdateModel(&t);
    scene_model_->Draw(shader_);

    lastframe_ = now;
}

void BallScene::OnMouseMotion(float x, float y)
{
    float dx, dy;

    dx = x - cursor_pos_.x;
    dy = y - cursor_pos_.y;

    cursor_pos_ = { x, y };

    camera_pos_.x += dx;
    camera_pos_.y += dy;

    LOGD("Mouse position: %f, %f\n", cursor_pos_.x/100, cursor_pos_.y/100);
}

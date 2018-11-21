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

void BallScene::DoFrame()
{
    uint64_t now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
    float dt = float(now - lastframe_) * 0.000000001f;

    GLuint scene_program = shader_->program_;
    glUseProgram(scene_program);
    glProgramUniformMatrix4fv(scene_program, 0, 1, GL_FALSE, glm::value_ptr(camera_.view_));
    glProgramUniformMatrix4fv(scene_program, 1, 1, GL_FALSE, glm::value_ptr(camera_.projection_));
    //glProgramUniform3fv(scene_program, 2, 1, glm::value_ptr(camera_.view_position_));
    glm::mat4 t = glm::translate(glm::mat4(1.0), glm::vec3(0, dt * 50, 0));
    scene_model_->UpdateModel(&t);
    scene_model_->Draw(shader_);

    lastframe_ = now;
}

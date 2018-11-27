#include "shader_scene.hpp"
#include <chrono>



void
ShaderScene::OnStartGraphics()
{
    std::vector<std::pair <GLenum, std::string >> shaderFilenames = { {GL_VERTEX_SHADER, "../src/shaders/shader_scene.vs.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "../src/shaders/shader_scene.fs.glsl"} };

    shader_ = new Shader(shaderFilenames);

    scene_model_ = new Model("../src/test/cube_object.obj");

    SceneManager* mgr = SceneManager::GetInstance();

    projection_ = glm::perspective(glm::radians(65.0f), mgr->GetScreenAspect(), 0.1f, 100.0f);

    view_position_ = glm::vec3(0.0, 1.0, -1.5);

    view_ = glm::lookAt(view_position_, glm::vec3(0), glm::vec3(0, 1, 0));

    g_spotlight.position = glm::vec3(0.0, 4.0, 0.0);
    g_spotlight.direction = glm::vec3(0.0, 0.0, 0.0)-g_spotlight.position;

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    lastframe_ = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

    animation_state_ = 3;
}

void
ShaderScene::DoFrame()
{
    uint64_t now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
    float dt = float(now - lastframe_) * 0.000000001f;

    if (animation_state_[0])
        time_.x += dt;
    if (animation_state_[1])
        time_.y += dt;

    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLuint scene_program = shader_->program_;
    glUseProgram(scene_program);
    glProgramUniformMatrix4fv(scene_program, 0, 1, GL_FALSE, glm::value_ptr(view_));
    glProgramUniformMatrix4fv(scene_program, 1, 1, GL_FALSE, glm::value_ptr(projection_));
    glProgramUniform3fv(scene_program, 2, 1, glm::value_ptr(view_position_));

    glProgramUniform2f(scene_program, 3, time_.x, time_.y);

    Lighting::FillSpotLight(scene_program, 6, &g_spotlight);
    scene_model_->Draw(shader_);

    lastframe_ = now;
}

void
ShaderScene::OnKillGraphics()
{
    CleanUp(&shader_);
    CleanUp(&scene_model_);
}
#include "instance_test.hpp"

void InstanceTest::OnStartGraphics()
{
    std::vector<std::pair <GLenum, std::string>> light_shaders = { {GL_VERTEX_SHADER, "../src/shaders/test.vs.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "../src/shaders/test.fs.glsl"} };
    shader_ = new gl00::Shader(light_shaders);

    instance_count_ = 3;

    scene_model_ = new gl00::Model("../src/test/cube_object.obj", instance_count_);

    gl00::SceneManager* mgr = gl00::SceneManager::GetInstance();

    projection_ = glm::perspective(glm::radians(45.0f), mgr->GetScreenAspect(), 0.1f, 100.0f);

    view_position_ = glm::vec3(0.f, 0.f, .5f);
    view_ = glm::lookAt(view_position_, glm::vec3(.15f, 0.f, 0.f), glm::vec3(0.0, 1.0, 0.0));
    p_model_ = new glm::mat4[instance_count_];

    for (int i = 0; i < instance_count_; i++)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3((float)i/8, 0.f, 0.f));
        model = glm::scale(model, glm::vec3(.1f));
        p_model_[i] = model;
    }
    scene_model_->UpdateModel(p_model_);
    //delete[] p_model;

    time_last_ns_ = TimeNs();
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
}

void InstanceTest::OnKillGraphics()
{
    CleanUp(&shader_);
    CleanUp(&scene_model_);
}

void InstanceTest::DoFrame()
{
    glClearColor(.5, .5, .5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_->program_);
    glProgramUniformMatrix4fv(shader_->program_, 0, 1, GL_FALSE, glm::value_ptr(view_));
    glProgramUniformMatrix4fv(shader_->program_, 1, 1, GL_FALSE, glm::value_ptr(projection_));


    scene_model_->Draw(shader_);
}

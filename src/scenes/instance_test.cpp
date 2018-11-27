#include "instance_test.hpp"

#define INSTANCE_COUNT 3

void InstanceTest::OnStartGraphics()
{
    last_frame_ns_ = 0;

    std::vector<std::pair <GLenum, std::string>> light_shaders = { {GL_VERTEX_SHADER, "../src/shaders/instance.vs.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "../src/shaders/light.fs.glsl"} };
    shader_ = new Shader(light_shaders);

    scene_model_ = new Model("../src/test/light_object.obj", INSTANCE_COUNT);

    SceneManager* mgr = SceneManager::GetInstance();

    projection_ = glm::perspective(glm::radians(45.0f), mgr->GetScreenAspect(), 0.1f, 100.0f);

    view_position_ = glm::vec3(0, 3.0, -1.5);
    view_ = glm::lookAt(view_position_, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
}

//glm::mat4 g_model[INSTANCE_COUNT];

void InstanceTest::DoFrame()
{
    glClearColor(.5, .5, .5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_->program_);
    glProgramUniformMatrix4fv(shader_->program_, 0, 1, GL_FALSE, glm::value_ptr(view_));
    glProgramUniformMatrix4fv(shader_->program_, 1, 1, GL_FALSE, glm::value_ptr(projection_));

    glm::mat4* p_model = new glm::mat4[INSTANCE_COUNT];

    for (int i = 0; i < INSTANCE_COUNT; i++)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3((float)i/10, 0, 0));
        model = glm::scale(model, glm::vec3(.1f));
        p_model[i] = model;
    }

    scene_model_->UpdateModel(p_model);
    scene_model_->Draw(shader_);
    delete[] p_model;
}

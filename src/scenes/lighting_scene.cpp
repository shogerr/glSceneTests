#include "lighting_scene.hpp"
#include <chrono>


void LightingScene::OnStartGraphics()
{
    last_frame_ns_ = 0;
    std::vector<std::pair <GLenum, std::string>> object_shaders = { {GL_VERTEX_SHADER, "../src/shaders/vertex.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "../src/shaders/fragment.glsl"} };

    std::vector<std::pair <GLenum, std::string>> light_shaders = { {GL_VERTEX_SHADER, "../src/shaders/light.vs.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "../src/shaders/light.fs.glsl"} };
    shaders_.push_back(new gl00::Shader(object_shaders));
    shaders_.push_back(new gl00::Shader(light_shaders));

    scene_model_ = new gl00::Model("../src/test/b_sphere2.obj");
    sphere_model_ = new gl00::Model("../src/test/two_spheres.obj");
    light_model_ = new gl00::Model("../src/test/sphere_object.obj", 3);

    light_state_ = 15;

    LOGI("number of meshes: %d\n", scene_model_->meshes_.size());

    gl00::SceneManager* mgr = gl00::SceneManager::GetInstance();
    
    projection_ = glm::perspective(glm::radians(65.0f), mgr->GetScreenAspect(), 0.1f, 100.0f);

    view_position_ = glm::vec3(0, 1.0, -2.5);
    view_ = glm::lookAt(view_position_, glm::vec3(0), glm::vec3(0, 1, 0));

    g_spotlight.pos_ = glm::vec3(0.0, 1.2, 0.0);
    g_spotlight.t->direction = glm::vec3(0.0, -1.0, 0.0);

    g_pointlights[0].t->position = glm::vec3(0.0, 0.2, 0.0);
    g_pointlights[0].t->light.diffuse = glm::vec3(1.0, 0.0, 0.0);
    g_pointlights[0].t->light.specular = glm::vec3(1.0, 0.0, 0.0);

    g_pointlights[1].pos_ = glm::vec3(0.25, 0.2, -1.0);
    g_pointlights[1].t->light.diffuse = glm::vec3(1.0, 1.0, 0.0);
    g_pointlights[1].t->light.specular = glm::vec3(1.0, 1.0, 0.0);

    g_object.SetAngularVelocity(glm::vec3(.9f));

    glBindVertexArray(light_model_->meshes_[0].vao_);

    glGenBuffers(1, &color_bo_);


    glBindBuffer(GL_ARRAY_BUFFER, color_bo_);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(7);
    glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
}

void LightingScene::DoFrame()
{
    uint64_t now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

    float dt = float(now - last_frame_ns_) * 0.000000001f;

    if (dt > 1)
        goto update_time;

    if (light_state_[3] == 0)
        dt = 0;

    light_pos.y += .01f * dt;

    GLuint scene_program = shaders_.front()->program_;
    GLuint light_program = shaders_.back()->program_;

    glClearColor(.5, .5, .5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0);

    g_object.UpdateTheta(dt);

    model = glm::rotate(model, g_object.Theta().x, glm::vec3(0.0, 1.0, 0.0));
    model = glm::translate(model, glm::vec3(glm::cos(g_object.Theta().x), .45,  0.0));
    model = glm::scale(model, glm::vec3(.25));

    scene_model_->UpdateModel(&model);
    //scene_model_->meshes_[0].UpdateModel(model);

    glUseProgram(scene_program);
    //LOGI("diffuse loc: %d\n", glGetUniformLocation(program, "diffuse"));
    glProgramUniformMatrix4fv(scene_program, 0, 1, GL_FALSE, glm::value_ptr(view_));
    glProgramUniformMatrix4fv(scene_program, 1, 1, GL_FALSE, glm::value_ptr(projection_));
    glProgramUniform3fv(scene_program, 2, 1, glm::value_ptr(view_position_));

    //g_pointlights[0].t->position = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, 0.001)) * glm::vec4(g_pointlights[0].t->position, 1.0);
    
    glm::mat4 lightmodel = glm::rotate(glm::mat4(1), g_object.Theta().x, glm::vec3(0.0, 1.0, 0.0));
    lightmodel = glm::translate(lightmodel, glm::vec3(0.0, 0.25, 1.0));

    if (light_state_[0] == 0)
        g_pointlights[0].t->position = glm::vec3(0, -100, 0);
    else
        g_pointlights[0].t->position =  lightmodel * glm::vec4(g_pointlights[0].pos_, 1.0);

    if (light_state_[1] == 0)
        g_pointlights[1].t->position = glm::vec3(0,-100,0);
    else
        g_pointlights[1].t->position = g_pointlights[1].pos_;
    if (light_state_[2] == 0)
    {
        g_spotlight.t->direction = glm::vec3(0, 1, 0);
        g_spotlight.t->position = glm::vec3(0, 100, 0);
    }
    else
    {
        g_spotlight.t->direction = glm::vec3(0, -1, 0);
        g_spotlight.t->position = g_spotlight.pos_;
    }
    gl00::Lighting::FillPointLight(scene_program, 6, *g_pointlights[0].t);
    gl00::Lighting::FillPointLight(scene_program, 13, *g_pointlights[1].t);

    gl00::Lighting::FillSpotLight(scene_program, 20, *g_spotlight.t);


    scene_model_->Draw(shaders_.front());
    sphere_model_->Draw(shaders_.front());

    glUseProgram(light_program);
    glProgramUniformMatrix4fv(light_program, 0, 1, GL_FALSE, glm::value_ptr(view_));
    glProgramUniformMatrix4fv(light_program, 1, 1, GL_FALSE, glm::value_ptr(projection_));

    glm::mat4* p_model = new glm::mat4[3];

    model = glm::translate(glm::mat4(1.0), g_spotlight.t->position);
    p_model[0] = glm::scale(model, glm::vec3(.08f));

    model = glm::translate(glm::mat4(1.0), g_pointlights[0].t->position);
    p_model[1] = glm::scale(model, glm::vec3(.08f));

    model = glm::translate(glm::mat4(1.0), g_pointlights[1].t->position);
    p_model[2] = glm::scale(model, glm::vec3(.08f));

    light_model_->UpdateModel(p_model);

    light_model_->Draw(shaders_.back());
    delete[] p_model;

update_time:
    last_frame_ns_ = now;
}

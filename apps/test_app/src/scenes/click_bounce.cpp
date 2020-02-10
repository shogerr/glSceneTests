#include "click_bounce.hpp"
#include <gl00/scene_manager.hpp>

ClickBounce::ClickBounce()
    : model_("assets/sphere_object.obj")
    , shader_(gl00::shader_paths{
        { GL_VERTEX_SHADER, std::filesystem::path("shaders/test.vs.glsl") },
        { GL_FRAGMENT_SHADER, std::filesystem::path("shaders/test.fs.glsl") }
        })
    , camera_({0.f,0.f,-5.f})
{}

void ClickBounce::OnStartGraphics()
{
    //scene_object_.solver_.force_.y = -9.8f;
    scene_object_.solver_.force_angular_.z = -.8f;
    time_ = TimeNs();
}

void ClickBounce::OnMouseMotion(float x, float y)
{
    gl00::MouseCoords last_coords = gl00::SceneManager::GetInstance().LastMouseCoords();
    //LOGD("mouse: %f, %f\n", last_coords.x, last_coords.y);
    camera_.Translate(.5f * (x - last_coords.x), {1.f,0.f,0.f});
    camera_.Translate(.5f * (y - last_coords.y), {0.f,1.f,0.f});
}

void ClickBounce::DoFrame()
{
    uint64_t now = TimeNs();
    //LOGD("Time: %llu\n", time_);
    //LOGD("Now:  %llu\n", now);
    //LOGD("(DT):  %llu\n", now-time_);
    float dt = (now - time_) * .000000001f;
    //LOGD("DT: %f\n", dt);
    //dt = dt * .000000001f;
    //LOGD("time scaled: %.*e\n", DECIMAL_DIG, dt);
    scene_object_.solver_.force_angular_.y = 10 * sin(now * .0000000001f);
    LOGD("force: %f\n", scene_object_.solver_.force_angular_.y);
    scene_object_.solver_.force_angular_.y = glm::clamp<float>(scene_object_.solver_.force_angular_.y, -5.f, 5.f);
    LOGD("clamped force: %f\n", scene_object_.solver_.force_angular_.y);
    scene_object_.Step(dt);
    //glm::mat4 matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f,.5f,0.f));
    glm::mat4 matrix = glm::rotate(glm::mat4(1.f), scene_object_.Theta().x, glm::vec3{1.f,0.f,0.f});
    matrix = glm::rotate(matrix, scene_object_.Theta().y, glm::vec3{0.f,1.f,0.f});
    matrix = glm::rotate(matrix, scene_object_.Theta().z, glm::vec3{0.f,0.f,1.f});
    matrix = glm::translate(matrix, scene_object_.Position());

    model_.UpdateModel(matrix);
    glUseProgram(shader_.Program());
    glProgramUniformMatrix4fv(shader_.Program(), 0, 1, GL_FALSE, glm::value_ptr(camera_.GetView()));
    glProgramUniformMatrix4fv(shader_.Program(), 1, 1, GL_FALSE, glm::value_ptr(camera_.GetProjection()));
    model_.Draw(shader_);
    time_ = now;
}
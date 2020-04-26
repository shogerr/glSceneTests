#include "click_bounce.hpp"
#include <gl00/scene_manager.hpp>

ClickBounce::ClickBounce()
    : ground_("assets/plane.obj")
    , model_("assets/sphere_object.obj")
    , shader_(gl00::shader_paths{
        { GL_VERTEX_SHADER, std::filesystem::path("shaders/test.vs.glsl") },
        { GL_FRAGMENT_SHADER, std::filesystem::path("shaders/test.fs.glsl") }
        })
    , camera_({0.f,5.f,-5.f})
{}

void ClickBounce::OnStartGraphics()
{
    //scene_object_.solver_.force_.y = -9.8f;
    scene_object_.Force(glm::vec3(0.f,-.2f,.0f));
    scene_object_.ForceAngular(glm::vec3(0.f,0.f,-.5f));

    scene_object_.Position(glm::vec3(0.f,3.f,0.f));
    time_ = TimeNs();
}

void ClickBounce::OnMouseMotion(float x, float y)
{
    gl00::MouseCoords last_coords = gl00::SceneManager::GetInstance().LastMouseCoords();
    //LOGD("mouse: %f, %f\n", last_coords.x, last_coords.y);
    camera_.Translate(.5f * (x - last_coords.x), {1.f,0.f,0.f});
    camera_.Translate(60.5f * (y - last_coords.y), {0.f,0.f,1.f});
}

// Equation for Plane
// ((x,y,z)-(Qx,Qy,Qz)) • (n_x,n_y,n_z) = 0
//
// Distance from point (P) to plane defined by a point Q on the plane, and the normal N.
// d = (P-Q) • N
bool Bounce(float radius, glm::vec3 point, glm::vec3 point_plane, glm::vec3 normal)
{
    float distance = glm::dot(normal, point - point_plane);
    //LOGD("%f\n", distance);

    return abs(distance) < radius;
}

unsigned int ClickBounce::ReadMessage()
{
    unsigned int message = 0;
    if (queue_.size() > 0)
    {
        message = queue_.front();
        LOGD("Message: %d\n", queue_.front());
        queue_.pop();
    }
    return message;
}

void ClickBounce::Poke(unsigned int value)
{
    queue_.push(static_cast<uint8_t>(value));
    LOGD("Pushed: %d\n", value);
}

float i = 0;

void ClickBounce::DoFrame()
{
    glm::vec3 force = glm::vec3(0.f,-9.8f,0.f);

    auto mouse = ReadMessage();
    switch (mouse)
    {
    case 1:
        LOGD("Adding force.");
        force.y -= 1.f;
        break;
    case 3:
        force.y += 1.f;
        break;
    default:
        break;
    }

    scene_object_.Force(force);

    // Do a rotation of the scene object around x-axis.
    auto matrix = glm::scale(glm::mat4(1.f), glm::vec3(1.f));
    matrix = glm::rotate(matrix, glm::pi<float>() * i, glm::vec3(1.f,0.f,0.f));
    ground_.UpdateModel(matrix);
    i += .05;

    uint64_t now = TimeNs();

    //LOGD("Time: %llu\n", time_);
    //LOGD("Now:  %llu\n", now);
    //LOGD("(DT):  %llu\n", now-time_);
    float dt = (now - time_) * .000000001f;
    //LOGD("DT: %f\n", dt);
    //dt = dt * .000000001f;
    //LOGD("time scaled: %.*e\n", DECIMAL_DIG, dt);
    float y = 10 * sin(now * .0000000001f);
    y = glm::clamp<float>(y, -.05f, .05f);
    auto F = scene_object_.ForceAngular();
    F.y = y;
    //LOGD("F.y: %f\n", F.y);
    scene_object_.ForceAngular(F);
    if (Bounce(1.f, scene_object_.Position(), glm::vec3(0.f), glm::vec3(0.f,1.f,0.f)))
    {
        //scene_object_.Force(glm::vec3(0.f,9.8f,0.f));
        auto v = scene_object_.Velocity();
        v.y = -v.y;
        scene_object_.Velocity(v);
    }

    scene_object_.Step(dt);

    //camera_.Target(scene_object_.Position());
    model_.UpdateModel(scene_object_.Matrix());


    glUseProgram(shader_.Program());
    glProgramUniformMatrix4fv(shader_.Program(), 0, 1, GL_FALSE, glm::value_ptr(camera_.GetView()));
    glProgramUniformMatrix4fv(shader_.Program(), 1, 1, GL_FALSE, glm::value_ptr(camera_.GetProjection()));
    model_.Draw(shader_);
    ground_.Draw(shader_);
    time_ = now;
}
#include "ground_scene.hpp"
#include <chrono>
#include "../../src/key_codes.hpp"
#include <filesystem>

GroundScene::GroundScene()
{}

void GroundScene::OnStartGraphics()
{
    gl00::shader_paths shaderFilenames = { {GL_VERTEX_SHADER, "shaders/ball.vs.glsl"},
                                           {GL_FRAGMENT_SHADER, "shaders/ball.fs.glsl"} };

    gl00::shader_paths light_shader_filenames = { {GL_VERTEX_SHADER, "shaders/light.vs.glsl"},
                                                  {GL_FRAGMENT_SHADER, "shaders/light.fs.glsl"} };
    shader_ = new gl00::Shader(shaderFilenames);
    light_shader_ = new gl00::Shader(light_shader_filenames);

    camera_pos_ = glm::vec3(.5f, 2.0f, -7.0f);
    camera_focus_ = glm::vec3(0.f, 1.f, 0.f);

    obstacles_ = { glm::vec3{-1.6f, 1.5f, 5.5f }, glm::vec3{ 1.5f, 1.5f, 3.5f }, glm::vec3{ 4.5f, 1.5f, 3.5f} };

    scene_model_ = new gl00::Model{ "assets/cube_object.obj" };
    obstacle_model_ = std::make_unique<gl00::Model>("assets/checkered_cube.obj", 3);
    light_model_ = new gl00::Model("assets/sphere_object.obj", 2);

    p_scene_model_ = std::make_unique<glm::mat4>(1.0f);

    scene_model_->UpdateModel(*p_scene_model_.get());

    obstacle_model_.get()->SetInstanceCount(obstacles_.size());

    p_obstacle_model = std::make_unique<glm::mat4[]>(obstacles_.size());

    // Set obstacle positions.
    for (unsigned int i = 0; i < obstacles_.size(); i++)
        p_obstacle_model[i] = glm::translate(glm::mat4(1.f), obstacles_[i].state_.position);
    obstacle_model_.get()->UpdateModel(p_obstacle_model);

    // Setup ground plane.
    ground_plane_ = new gl00::Model{ "assets/plane.obj" };

    ground_plane_model_ = glm::scale(glm::mat4(1.f), glm::vec3(15.f, 1.f, 15.f));
    ground_plane_model_ = glm::translate(ground_plane_model_, glm::vec3(0.f, 1.5f, 0.f));

    ground_plane_->UpdateModel(ground_plane_model_);

    figure_solver_.state_.position = glm::vec3(0.1f, 1.5f, 0.1f);

    // Set target for figure
    target_ = {0.0f, 1.5f, 0.0f};


    point_lights_.push_back(gl00::Lighting::PointLight{});
    point_lights_.back().position = {-9.f, 2.5f, 2.5f};

    point_lights_.push_back(gl00::Lighting::PointLight{});
    point_lights_.back().position = {9.f, 2.0f, 2.5f};

    p_light_model_ = new glm::mat4[point_lights_.size()];
    for (unsigned int i = 0; i < point_lights_.size(); i++)
    {
        glm::mat4 m = glm::translate(glm::mat4(1.f), point_lights_[i].position);
        p_light_model_[i] = glm::scale(m, glm::vec3(.25f));
    }
    light_model_->UpdateModel(*p_light_model_);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    lastframe_ = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}

void GroundScene::Step()
{
    camera_.View(glm::lookAt(camera_pos_, glm::vec3(figure_solver_.state_.position.x, 1.f, figure_solver_.state_.position.z), glm::vec3(0.f, 1.f, 0.f)));
}

void GroundScene::OnKillGraphics()
{
    CleanUp(&shader_);
    CleanUp(&light_shader_);
}

void GroundScene::DoFrame()
{
    //glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    uint64_t now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
    float dt = float(now - lastframe_) * 0.000000001f;
    Step();
    GLuint scene_program = shader_->Program();
    gl00::StateSolver* solver = &figure_solver_;

    glUseProgram(scene_program);
    glProgramUniformMatrix4fv(scene_program, 0, 1, GL_FALSE, glm::value_ptr(camera_.GetView()));
    glProgramUniformMatrix4fv(scene_program, 1, 1, GL_FALSE, glm::value_ptr(camera_.GetProjection()));
    glProgramUniform3fv(scene_program, 2, 1, glm::value_ptr(camera_.GetView()));

    gl00::Lighting::FillSpotLight(scene_program, 6, spotlight_);
    gl00::Lighting::FillPointLight(scene_program, 16, point_lights_[0]);
    gl00::Lighting::FillPointLight(scene_program, 23, point_lights_[1]);

    solver->force_.y = gravity_;
    CalcGroundHit(figure_solver_, 1.f, dt);
    for (auto &o : obstacles_)
    {
        CalcGroundHit(o, .5f, dt);
        o.force_.y = gravity_;
    }

    glm::vec3 obstacle_force = ObstacleForce();

    solver->force_.z = -1.f * (solver->state_.position.z - target_.z) - 1.f*solver->state_.velocity.z;// +5.f*obstacle_force.z;
    solver->force_.x = -1.f * (solver->state_.position.x - target_.x) - 1.f*solver->state_.velocity.x;// +5.f*obstacle_force.x;

    *p_scene_model_ = glm::scale(glm::mat4(1.0), glm::vec3(1.0));
    *p_scene_model_ = glm::translate(*p_scene_model_, figure_solver_.state_.position);

    scene_model_->Draw(*shader_);

    ground_plane_->Draw(*shader_);

    for (unsigned int i = 0; i < obstacles_.size(); i++)
    {
        p_obstacle_model[i] = glm::translate(glm::mat4(1.f), obstacles_[i].state_.position + glm::vec3(0.f,-.5f,0.f));
    }

    obstacle_model_.get()->Draw(*shader_);
    glUseProgram(light_shader_->Program());
    glProgramUniformMatrix4fv(light_shader_->Program(), 0, 1, GL_FALSE, glm::value_ptr(camera_.GetView()));
    glProgramUniformMatrix4fv(light_shader_->Program(), 1, 1, GL_FALSE, glm::value_ptr(camera_.GetProjection()));
    light_model_->Draw(*light_shader_);

    lastframe_ = now;
}

void GroundScene::SpawnObstacle()
{
    // Add obstacle point
    glm::vec3 obstacle{glm::linearRand(-15.0f, 15.0f), 1.5f, glm::linearRand(-15.f, 15.f)};
    obstacles_.push_back(gl00::StateSolver{ obstacle });
    obstacles_.back().force_.y = 500.f;

    int i = 0;
    for (auto o : obstacles_)
    {
        p_obstacle_model[i] = glm::translate(glm::mat4(1.f), obstacles_[i].state_.position);
        i++;
    }
    obstacle_model_.get()->SetInstanceCount(obstacles_.size());
    obstacle_model_.get()->UpdateModel(p_obstacle_model);
}

glm::vec3 GroundScene::ObstacleForce()
{
    glm::vec3 force{ 0.0f, 0.0f, 0.0f };
    for (auto o : obstacles_)
    {
        glm::vec3 d = figure_solver_.state_.position - o.state_.position;
        if (glm::length(d) < 1.5f)
        {
            if (glm::lessThan(glm::abs(d), glm::vec3(.1f)).x)
            {
               if (d.x < 0.1f)
                    d.x = -.1f;
               else
                    d.x = .1f;
            }
            if (glm::lessThan(glm::abs(d), glm::vec3(.1f)).z)
            {
                if (d.z < 0.1f)
                    d.z = -.1f;
                else
                    d.z = .1f;
            }
            force += glm::vec3(1.f) / (d);
        }
    }
    return force;
}

void GroundScene::Poke(unsigned int i)
{ 
    switch (i)
    {
    case GL00K_a:
        figure_solver_.force_.x += 50.f;
        break;
    case GL00K_d:
        figure_solver_.force_.x += -50.f;
        break;
    case GL00K_s:
        figure_solver_.force_.z += -50.f;
        break;
    case GL00K_w:
        figure_solver_.force_.z += 50.f;
        break;
    case GL00K_b:
        SpawnObstacle();
        break;
    }
}

void GroundScene::OnMouseMotion(float x, float y)
{
    float dx, dy;

    dx = x - cursor_pos_.x;
    dy = y - cursor_pos_.y;

    cursor_pos_ = { x, y };

    camera_pos_.x += 5.f * dx;
    camera_pos_.y += 5.f * dy;
}

void GroundScene::CalcGroundHit(gl00::StateSolver &solver, float cr, float dt)
{
    while (dt > EPSILON)
    {
        float tmin = dt;
        bool hit = false;
        gl00::StateSolver::State* state = &solver.state_;

        float pos_sqrt = sqrt(state->velocity.y * state->velocity.y - 2.f * solver.force_.y * (state->position.y-1.f));
        float tground = (-1.f*state->velocity.y - pos_sqrt) / solver.force_.y;
        float tground_ = (-1.f*state->velocity.y + pos_sqrt) / solver.force_.y;

        if (tground > 0.f && tground < tmin)
        {
            tmin = tground;
            hit = true;
        }
        if (tground_ > 0.f && tground_ < tmin)
        {
            tmin = tground_;
            hit = true;
        }

        solver.Simulate(tmin);

        if (state->position.y < 1.f)
            state->position.y = 1.f;
        if (hit)
        {
            state->velocity.y *= -cr;
        }
        else
            return;

        dt -= tmin;
    }
}
#include "bezier_scene.hpp"
#include <chrono>

void BezierScene::OnStartGraphics()
{
    std::vector<std::pair <GLenum, std::string >> shaderFilenames = { {GL_VERTEX_SHADER, "../src/shaders/bezier.vs.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "../src/shaders/bezier.fs.glsl"} };

    line_shader_ = new gl00::Shader(shaderFilenames);

    std::vector<std::pair <GLenum, std::string >> control_shader_filenames = { {GL_VERTEX_SHADER, "../src/shaders/bezier-point.vs.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "../src/shaders/bezier-point.fs.glsl"} };

    control_shader_ = new gl00::Shader(control_shader_filenames);

    timer_ = 0;
    rot_timer_ = timer_;
    animation_state_ = 0;

    view_ = glm::lookAt(glm::vec3(0.0,0.0,1.0), glm::vec3(0.0), glm::vec3(0, 1, 0));

    gl00::SceneManager* mgr = gl00::SceneManager::GetInstance();
    //projection_ = glm::perspective(glm::radians(45.0f), mgr->GetScreenAspect(), 0.1f, 100.0f);
    projection_ = glm::ortho(-256.0f, 256.0f, -256.0f, 256.0f, -350.0f, 350.0f);

    lastframe_ns_ = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
    glEnable(GL_PROGRAM_POINT_SIZE);

    Line l = Line{};
    LOGI("resolution: %d", l.resolution_);

    int d = 9;
    std::vector<glm::vec3> cp = {};
    for (int i = d; i >= 0; i--)
    {
        float theta = 2*glm::pi<float>()/d * i;
        cp.push_back(glm::vec3(glm::cos(theta), glm::sin(theta), -1 * i/(float)d + -.05f));
    }

    l.control_points_ = cp;
    l.Setup();

    lines_.push_back(l);

    const int count = 10;
    glm::mat4 mvps[count];
    for (int i = 0; i < count; i++)
    {
        mvps[i] = glm::scale(glm::mat4(1.0), glm::vec3(175.0));
        mvps[i] = glm::rotate(mvps[i], 2 * glm::pi<float>() * i / count, glm::vec3(0, 0, 1));
        //mvps[i] = glm::rotate(glm::mat4(1.0), 2 * glm::pi<float>() * i / count, glm::vec3(0, 0, 1));
    }
    glGenBuffers(1, &linebo_);
    glBindBuffer(GL_ARRAY_BUFFER, linebo_);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), &mvps[0], GL_DYNAMIC_DRAW);

    glBindVertexArray(l.vao_);

    for (int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(1+i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*i));
        glEnableVertexAttribArray(1+i);
        glVertexAttribDivisor(1+i, 1);
    }

    glm::vec4 colors[1000];

    for (int i = 0; i < 1000; i++)
    {
        colors[i] = glm::vec4(.001*(i+1), 1.0-(.001*(i+1)), 0.5, 1.0);
    }

    glGenBuffers(1, &colorbo_);
    glBindBuffer(GL_ARRAY_BUFFER, colorbo_);
    glBufferData(GL_ARRAY_BUFFER, l.resolution_ * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);

    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glEnableVertexAttribArray(5);
    //glVertexAttribDivisor(4, 1);
    glBindVertexArray(0);

    l = Line{};
    cp = {};
    d = 5;
    for (int i = d; i >= 0; i--)
    {
        float theta = 2*glm::pi<float>()/d * i;
        cp.push_back(glm::vec3(glm::cos(theta), glm::sin(theta), i/(float)d - 1/5));
    }

    l.control_points_ = cp;
    l.Setup();
    lines_.push_back(l);
    glBindVertexArray(l.vao_);

    glBindBuffer(GL_ARRAY_BUFFER, linebo_);
    for (int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(1+i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*i));
        glEnableVertexAttribArray(1+i);
        glVertexAttribDivisor(1+i, 1);
    }
    glBindBuffer(GL_ARRAY_BUFFER, colorbo_);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glEnableVertexAttribArray(5);
    glBindVertexArray(0);


    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
}

void BezierScene::DoFrame()
{
    uint64_t now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
    float dt = float(now - lastframe_ns_) * 0.000000001f;
    if (animation_state_[5])
        dt = 0;

    timer_ += dt;

    int instance_count = 10;

    if (animation_state_[4])
    {
        rot_timer_ += dt;
        view_ = glm::lookAt(glm::vec3(glm::sin(.25*rot_timer_), 0.0, glm::cos(.25*rot_timer_)), glm::vec3(0.0), glm::vec3(0, 1, 0));
    }

    Line l = lines_.at(0);
    Line l_2 = lines_.at(1);

    l.control_points_[2].y = glm::sin(timer_);
    l.control_points_[3].x = glm::sin(1.5f*timer_);
    l.control_points_[6].y = -1.0f*glm::sin(timer_);
    l.control_points_[0].x = glm::cos(timer_);
    l.control_points_[9].y = -1.0f*glm::cos(timer_);
    l.control_points_[0].z = -.5f*glm::cos(timer_)-1.5f;

    l.UpdateLine();
    l_2.UpdateLine();
    glUseProgram(line_shader_->program_);
    glBindVertexArray(l.vao_);

    glProgramUniformMatrix4fv(line_shader_->program_, 0, 1, GL_FALSE, glm::value_ptr(view_));
    glProgramUniformMatrix4fv(line_shader_->program_, 1, 1, GL_FALSE, glm::value_ptr(projection_));

    if (animation_state_[3])
        instance_count = 1;

    glDrawArraysInstanced(GL_LINE_STRIP, 0, l.resolution_, instance_count);

    glBindVertexArray(l_2.vao_);

    glDrawArraysInstanced(GL_LINE_STRIP, 0, l_2.resolution_, instance_count);

    glUseProgram(control_shader_->program_);

    glProgramUniformMatrix4fv(control_shader_->program_, 0, 1, GL_FALSE, glm::value_ptr(view_));
    glProgramUniformMatrix4fv(control_shader_->program_, 1, 1, GL_FALSE, glm::value_ptr(projection_));

    if (animation_state_[2])
        instance_count = 1;
    if (animation_state_[0])
    {
        glBindVertexArray(l.vao_);
        glDrawArraysInstanced(GL_POINTS, l.resolution_, l.control_points_.size(), instance_count);
        glBindVertexArray(l_2.vao_);
        glDrawArraysInstanced(GL_POINTS, l_2.resolution_, l_2.control_points_.size(), instance_count);
    }
    if (animation_state_[1])
    {
        glBindVertexArray(l.vao_);
        glDrawArraysInstanced(GL_LINE_STRIP, l.resolution_, l.control_points_.size(), instance_count);
        glBindVertexArray(l_2.vao_);
        glDrawArraysInstanced(GL_LINE_STRIP, l_2.resolution_, l_2.control_points_.size(), instance_count);
    }

    lastframe_ns_ = now;
}

void BezierScene::OnKillGraphics()
{
    CleanUp(&control_shader_);
    CleanUp(&line_shader_);
}

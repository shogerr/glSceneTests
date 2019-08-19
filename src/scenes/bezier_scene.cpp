#include "bezier_scene.hpp"
#include <chrono>

gl00::scenes::BezierScene::BezierScene() : timer_(0.f), rot_timer_(timer_), animation_state_(0), lastframe_ns_(0) {}

void gl00::scenes::BezierScene::OnStartGraphics()
{
    using ShaderList = std::vector<std::pair <GLenum, std::string >>;

    ShaderList curve_shaders { {GL_VERTEX_SHADER, "shaders/bezier.vs.glsl"},
                               {GL_FRAGMENT_SHADER, "shaders/bezier.fs.glsl"} };

    ShaderList control_shaders { {GL_VERTEX_SHADER, "shaders/bezier-point.vs.glsl"},
                                 {GL_FRAGMENT_SHADER, "shaders/bezier-point.fs.glsl"} };

    line_shader_ = std::make_unique<gl00::Shader>(curve_shaders);
    control_shader_ = std::make_unique<gl00::Shader>(control_shaders);

    animation_state_[4] = 1;

    view_ = glm::lookAt(glm::vec3(0.0,0.0,1.0), glm::vec3(0.0), glm::vec3(0, 1, 0));

    //gl00::SceneManager& mgr = gl00::SceneManager::GetInstance();
    //projection_ = glm::perspective(glm::radians(65.0f), mgr.GetScreenAspect(), 0.01f, 1000.0f);
    projection_ = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, .01f, 100.0f);

    lastframe_ns_ = TimeNs();

    glEnable(GL_PROGRAM_POINT_SIZE);

    gl00::Line l;

    int d = 9;
    std::vector<glm::vec3> cp (0);
    for (int i = d; i >= 0; i--)
    {
        float theta = 2*glm::pi<float>()/d * i;
        cp.push_back(glm::vec3(glm::cos(theta), glm::sin(theta), -1 * i/(float)d + -.05f));
    }

    l.control_points_ = cp;

    lines_.push_back(l);

    lines_.back().Setup();

    std::vector<glm::mat4> mvps(10);
    int i = 0;
    for (auto& m : mvps)
    {
        m = glm::scale(glm::mat4(1.0), glm::vec3(.85f));
        m = glm::rotate(m, 2 * glm::pi<float>() * i / mvps.size(), glm::vec3(0, 0, 1));
        i++;
    }

    glGenBuffers(1, &linebo_);
    glBindBuffer(GL_ARRAY_BUFFER, linebo_);
    glBufferData(GL_ARRAY_BUFFER, mvps.size() * sizeof(glm::mat4), &mvps[0], GL_DYNAMIC_DRAW);

    glBindVertexArray(lines_.back().vao_);

    for (int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(1+i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*i));
        glEnableVertexAttribArray(1+i);
        glVertexAttribDivisor(1+i, 1);
    }

    std::vector<glm::vec4> colors(1000);

    for (int i = 0; i < 1000; i++)
        colors[i] = glm::vec4(.001*(i+1.), 1.0-(.001*(i+1.)), 0.5, 1.0);
 
    glGenBuffers(1, &colorbo_);
    glBindBuffer(GL_ARRAY_BUFFER, colorbo_);
    glBufferData(GL_ARRAY_BUFFER, l.GetResolution() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);

    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glEnableVertexAttribArray(5);
    //glVertexAttribDivisor(4, 1);
    glBindVertexArray(0);

    cp.clear();

    d = 5;
    for (int i = d; i >= 0; i--)
    {
        float theta = 2*glm::pi<float>()/d * i;
        cp.push_back(glm::vec3(glm::cos(theta), glm::sin(theta), i/(float)d - 1/5));
    }

    l.control_points_ = cp;

    lines_.push_back(l);
    lines_.back().Setup();
    glBindVertexArray(lines_.back().vao_);

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

void gl00::scenes::BezierScene::DoFrame()
{
    uint64_t now = TimeNs();
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

    gl00::Line& l = lines_.at(0);
    gl00::Line& l_2 = lines_.at(1);

    l.control_points_[2].y = glm::sin(timer_);
    l.control_points_[3].x = glm::sin(1.5f*timer_);
    l.control_points_[6].y = -1.f*glm::sin(timer_);
    l.control_points_[0].x = glm::cos(timer_);
    l.control_points_[9].y = -1.f*glm::cos(timer_);
    l.control_points_[0].z = -.5f*glm::cos(timer_)-1.5f;

    l.Update();
    l_2.Update();
    glUseProgram(line_shader_->program_);
    glBindVertexArray(l.vao_);

    glProgramUniformMatrix4fv(line_shader_->program_, 0, 1, GL_FALSE, glm::value_ptr(view_));
    glProgramUniformMatrix4fv(line_shader_->program_, 1, 1, GL_FALSE, glm::value_ptr(projection_));

    if (animation_state_[3])
        instance_count = 1;

    glDrawArraysInstanced(GL_LINE_STRIP, 0, l.GetResolution(), instance_count);

    glBindVertexArray(l_2.GetVAO());

    glDrawArraysInstanced(GL_LINE_STRIP, 0, l_2.GetResolution(), instance_count);

    glUseProgram(control_shader_->program_);

    glProgramUniformMatrix4fv(control_shader_->program_, 0, 1, GL_FALSE, glm::value_ptr(view_));
    glProgramUniformMatrix4fv(control_shader_->program_, 1, 1, GL_FALSE, glm::value_ptr(projection_));

    if (animation_state_[2])
        instance_count = 1;
    if (animation_state_[0])
    {
        glBindVertexArray(l.GetVAO());
        glDrawArraysInstanced(GL_POINTS, l.GetResolution(), static_cast<int>(l.control_points_.size()), instance_count);
        glBindVertexArray(l_2.GetVAO());
        glDrawArraysInstanced(GL_POINTS, l_2.GetResolution(), static_cast<int>(l_2.control_points_.size()), instance_count);
    }
    if (animation_state_[1])
    {
        glBindVertexArray(l.GetVAO());
        glDrawArraysInstanced(GL_LINE_STRIP, l.GetResolution(), static_cast<int>(l.control_points_.size()), instance_count);
        glBindVertexArray(l_2.GetVAO());
        glDrawArraysInstanced(GL_LINE_STRIP, l_2.GetResolution(), static_cast<int>(l_2.control_points_.size()), instance_count);
    }

    lastframe_ns_ = now;
}

void gl00::scenes::BezierScene::OnKillGraphics()
{
    for (int i = 0; i < 5; i++)
    {
        glDisableVertexAttribArray(i);
    }

    glDeleteBuffers(1, &linebo_);
    glDeleteBuffers(1, &colorbo_);

    for (auto& l : lines_)
        l.Clean();
}

void gl00::scenes::BezierScene::OnUnInstall()
{
}

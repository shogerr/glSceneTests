#include "line_test.hpp"
#include <iostream>
#include "../gizmos/lighting.hpp"


gl00::scenes::LineTest::LineTest() : camera_({ 0.f,0.f,3.f }, glm::vec3(0.f))
{

}

void gl00::scenes::LineTest::SingleLine()
{
    auto print_cp = [](std::vector<glm::vec3>& vs) {
        for (auto v : vs) { std::cout << v.x << "," << v.y << "," << v.z << std::endl; } };

    line_.Setup();

    print_cp(line_.control_points_);

    glGenBuffers(1, &bo_);
    glBindBuffer(GL_ARRAY_BUFFER, bo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), glm::value_ptr(glm::mat4(1.f)), GL_DYNAMIC_DRAW);

    glBindVertexArray(line_.vao_);

    for (int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(1+i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*i));
        glEnableVertexAttribArray(1+i);
        glVertexAttribDivisor(1+i, 1);
    }
    glEnableVertexAttribArray(1);

}

void gl00::scenes::LineTest::OnStartGraphics()
{
    using ShaderList = std::vector<std::pair<GLenum, std::string>>;

    ShaderList shaders { {GL_VERTEX_SHADER, "shaders/line.vs.glsl"},
                         {GL_FRAGMENT_SHADER, "shaders/line.fs.glsl"} };

    line_shader_ = std::make_unique<gl00::Shader>(shaders);

    glEnable(GL_PROGRAM_POINT_SIZE);

    SingleLine();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void gl00::scenes::LineTest::DoFrame()
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    line_.Update();
    glUseProgram(line_shader_->program_);

    glBindVertexArray(line_.vao_);

    glProgramUniformMatrix4fv(line_shader_->program_, 0, 1, GL_FALSE, glm::value_ptr(camera_.GetView()));
    glProgramUniformMatrix4fv(line_shader_->program_, 1, 1, GL_FALSE, glm::value_ptr(camera_.GetProjection()));

    glDrawArraysInstanced(GL_LINE_STRIP, 0, line_.GetResolution(), 1);
}

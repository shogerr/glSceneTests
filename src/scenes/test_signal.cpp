#include "test_signal.hpp"
#include <gl00/common.hpp>
#include <string>

const std::string vertex_shader = R"(
layout (location = 0) in vec3 position;
void main() {
    gl_Position = vec4(position, 1.0);
}
)";

const std::string fragment_shader = R"(
out vec4 color;
void main() {
    color = vec4(1.0,1.0,1.0,1.0);
}
)";

gl00::scenes::TestSignal::TestSignal() {}

gl00::scenes::TestSignal::~TestSignal() { LOGD("Destroying Test Signal.\n"); }

void gl00::scenes::TestSignal::OnStartGraphics()
{
    std::vector<glm::vec3> triangle = {
        {-.5f,-.5f,0.f},{.5f,-.5f,0.f},{0.f,.5f,0.f},{-.5f,-.5f,0.f}
    };

    for (auto p: triangle)
        points_.push_back(p);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, points_.size()*sizeof(glm::vec3), &points_[0], GL_STATIC_DRAW);
    glPointSize(5.f);
    glLoadIdentity();
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void gl00::scenes::TestSignal::DoFrame()
{
    glClearColor(.5f,.5f,.5f,1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_LINES, 0, points_.size());
    glDisableVertexAttribArray(0);
}

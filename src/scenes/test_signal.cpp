#include "test_signal.hpp"
#include <gl00/common.hpp>

gl00::scenes::TestSignal::TestSignal() {}

gl00::scenes::TestSignal::~TestSignal() { LOGD("Destroying Test Signal.\n"); }

void gl00::scenes::TestSignal::OnStartGraphics()
{
    std::vector<glm::vec3> triangle = {
        {-1.f,-1.f,0.f},{1.f,-1.f,0.f},{0.f,1.f,0.f}
    };

    for (auto p: triangle)
        points_.push_back(p);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, points_.size()*sizeof(glm::vec3), &points_[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void gl00::scenes::TestSignal::DoFrame()
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

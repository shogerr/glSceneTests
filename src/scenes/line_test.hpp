#pragma once

#include <gl00/scene.hpp>
#include <gl00/shader.hpp>
#include "../gizmos/line.hpp"
#include <gl00/camera.hpp>

#include <memory>
#include <vector>

namespace gl00
{
    namespace scenes
    {
        class LineTest : public gl00::Scene
        {
        public:
            LineTest();

            void OnStartGraphics();
            void DoFrame();
        private:
            void SingleLine();

            glm::mat4 model_;

            GLuint bo_;

            std::unique_ptr<gl00::Shader> line_shader_;
            gl00::Camera camera_{ {0.f,0.f,1.f}, glm::vec3(0.f) };

            gl00::Line line_;
            std::vector<gl00::Line> lines_;
        };
    }
}
#pragma once

#include <gl00/engine_parts.hpp>
#include "../gizmos/line.hpp"
#include <vector>
#include <bitset>

namespace gl00
{
    namespace scenes
    {
        class BezierScene : public gl00::Scene
        {
        public:
            BezierScene();
            void OnStartGraphics();
            void DoFrame();
            void OnKillGraphics();
            void OnUnInstall();
            void Poke(unsigned int e) { animation_state_ ^= 1ULL << e; }
        private:
            GLuint linebo_{ 0 };
            GLuint colorbo_{ 0 };

            std::unique_ptr<gl00::Shader> control_shader_;
            std::unique_ptr<gl00::Shader> line_shader_;

            glm::mat4 projection_{ 0 };
            glm::mat4 view_{ 0 };

            std::bitset<6> animation_state_;

            std::vector<gl00::Line> lines_;

            float timer_;
            float rot_timer_;

            uint64_t lastframe_ns_;
        };
    }

}

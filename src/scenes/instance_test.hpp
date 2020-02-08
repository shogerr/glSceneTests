#ifndef __INSTANCE_TEST_HPP
#define __INSTANCE_TEST_HPP

#include <memory>
#include <gl00/engine_parts.hpp>
#include <gl00/camera.hpp>

namespace gl00
{
    namespace scenes
    {
        class InstanceTest : public gl00::Scene
        {
        public:
            InstanceTest();
            ~InstanceTest();
            void DoFrame();
            void OnStartGraphics();
            void OnKillGraphics();
            void OnUnInstall();
            void OnMouseMotion(float x, float y);
        private:
            int instance_count_;

            std::unique_ptr<gl00::Shader> shader_;

            gl00::Camera camera_;
            glm::vec2 mouse_coords_ {0.f};

            gl00::Model cube_model_;
            gl00::Model sphere_model_;

            glm::vec3 view_position_;
            glm::mat4 projection_;
            glm::mat4 view_;

            uint64_t time_last_ns_;
        };
    }
}

#endif // !__INSTANCE_TEST_HPP


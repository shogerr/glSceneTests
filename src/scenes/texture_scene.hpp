#pragma once

#include <gl00/engine_parts.hpp>
#include <gl00/model.hpp>

namespace gl00
{
    namespace scenes
    {
        class TextureScene : public gl00::Scene
        {
        public:
            typedef struct particle {
                glm::vec3 s;
                glm::vec3 v;

                float e;
            } Particle;

            void OnStartGraphics();
            void DoFrame();

            TextureScene();

            void SetTextureView(int scene_view);

        private:
            uint64_t last_frame_ns_;
            std::unique_ptr<gl00::Model> sphere_model_;
            std::unique_ptr<gl00::Shader> shader_;

            GLuint emptytex_;
            GLuint scenetex_;

            float theta_;
            int dir_;

            int texture_scene_;

            Particle focus_;

            void Step();
        };
    }
}

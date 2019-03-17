#ifndef __SCATTERING_SCENE_HPP
#define __SCATTERING_SCENE_HPP

#include <gl00/engine_parts.hpp>
#include <gl00/model.hpp>
#include <gl00/camera.hpp>

namespace gl00::scenes
{
    class ScatteringScene : public gl00::Scene
    {
    public:
        //ScatteringScene();
        void OnStartGraphics();
        void DoFrame();
        void Reshape();
    private:
        uint64_t time_last_ns_;
        float counter_;
        float theta_;
        glm::mat4 rotation_;

        gl00::Model* scene_model_;

        gl00::Shader* shaders_;
        gl00::Shader* depth_buffer_shader_;

        gl00::Camera camera_;
        glm::vec3 camera_pos_;
        glm::vec3 camera_focus_;

        glm::vec3 light_pos_;

        GLuint depth_pass_texture_;
        static const GLuint depth_pass_texture_size_;

        glm::mat4 depth_pass_matrix_;
        GLuint depth_pass_matrix_location_;
        static const glm::mat4 depth_pass_bias_matrix_;

        GLuint fbo_;
        GLuint vao_;
    };


}

#endif // !__SCATTERING_SCENE_HPP

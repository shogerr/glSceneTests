#ifndef __SCATTERING_SCENE_HPP
#define __SCATTERING_SCENE_HPP

#include <gl00/engine_parts.hpp>

#ifndef HEADER_OBJ_LOADER
#include <gl00/model_single.hpp>
#endif

#include <gl00/camera.hpp>


namespace gl00::scenes
{
    class ScatteringScene : public gl00::Scene
    {
    public:
        void OnStartGraphics();
        void DoFrame();
        void OnScreenResize(int width, int height);
        void Poke(int i);
    private:
        uint64_t time_last_ns_;
        float counter_;
        float theta_;
        glm::mat4 rotation_;

#ifndef HEADER_OBJ_LOADER
        gl00::ModelSingle* scene_model_;
#endif

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

        float fovy_;

        // Scene Control
        bool pause_;

        // Shader Uniforms
        float uniform_width_{ .5f };
        float uniform_wrap_{ .5f };
        float uniform_falloff_{ 5.0f };

        int width_{ 640 };
        int height_{ 480 };

        // Depth Pass Buffers
        GLuint fbo_;
        GLuint vao_;

        GLuint vertex_count_;

        // Model Buffers
        GLuint dragon_vao_;
        GLuint dragon_vert_vbo_;
        GLuint dragon_norm_vbo_;
    };
}

#endif // !__SCATTERING_SCENE_HPP

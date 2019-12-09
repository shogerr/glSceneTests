#pragma once

#include <memory>
#include "scene.hpp"

namespace gl00
{
    class Engine {
    public:
        Engine();
        ~Engine();

        void DoFrame();
        static Engine& GetInstance();

        void SetScreenDimensions(int width, int height);
        void SetDefaultScene(std::unique_ptr<gl00::Scene> scene);

    private:
        bool first_frame_;
        bool has_globjects_;

        int window_height_;
        int window_width_;

        bool PreRender();
        bool InitGLObjects();

        void ConfigureOpenGL();
        void KillContext();
        void KillGlObjects();

        std::unique_ptr<gl00::Scene> default_scene_;
    };
}

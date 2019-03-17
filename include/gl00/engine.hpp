#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#include "scene.hpp"

namespace gl00
{
    class Engine {
    public:
        Engine();
        Engine(gl00::Scene* scene);
        ~Engine();

        void DoFrame();
        static Engine* GetInstance();

        void SetScreenDimensions(int width, int height);
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

        gl00::Scene* default_scene_;
    };
}


#endif // __ENGINE_HPP

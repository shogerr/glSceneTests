#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#include "common.hpp"

namespace gl00
{
    class Engine {
    public:
        Engine();
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
    };
}


#endif // __RENDERER.HPP
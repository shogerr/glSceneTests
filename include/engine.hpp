#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#include "common.hpp"
#include <glm/ext.hpp>
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
        bool preRender();
        bool initGLObjects();
        bool first_frame_;
        bool has_globjects_;

        int mWindowHeight;
        int mWindowWidth;

        void ConfigureOpenGL();
        void KillContext();
        void KillGlObjects();
    };
}


#endif // __RENDERER.HPP

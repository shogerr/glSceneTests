#pragma once

#ifdef _WIN32
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>
#endif

#if defined(__linux__)
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif

#include <memory>

namespace gl00
{
    using SDLWindow = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

    void SDLInit(std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> & window, SDL_GLContext &context, int width, int height);

    class SDLApp
    {
    public:
        SDLApp(int width = 768, int height = 768) : window_(nullptr, SDL_DestroyWindow) {
            gl00::SDLInit(window_, context_, width, height);
            SDL_GL_SetSwapInterval(1);
        }
        void Quit() {
            SDL_GL_DeleteContext(context_);
            SDL_Quit();
        }
        void Swap() { SDL_GL_SwapWindow(window_.get()); }
    private:
        SDLWindow window_;
        SDL_GLContext context_;
    };


    void _SDL_CheckError(int line = -1);

    void _SDL_Init(std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> & window, SDL_GLContext &context, int width, int height);
}

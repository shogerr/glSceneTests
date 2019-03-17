#include <SDL.h>
#include <memory>

namespace gl00
{
    void _SDL_CheckError(int line = -1);

    void _SDL_Init(std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> & window, SDL_GLContext &context, int width, int height);
}

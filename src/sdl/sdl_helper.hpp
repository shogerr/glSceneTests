#include <SDL.h>

namespace gl00
{
    void _SDL_CheckError(int line = -1);

    void _SDL_Init(SDL_Window *&window, SDL_GLContext &context, int width, int height);
}

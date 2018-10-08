#ifndef __ENGINE_HPP
#define _ENGINE_HPP

#include "common.hpp"
#include <glm/ext.hpp>

class Engine {
public:
    Engine();
    ~Engine();

    void cycle();
    void doFrame();
    static Engine* getInstance();

    void setScreenDimensions(int width, int height);
private:
    bool preRender();
    bool initGLObjects();
    bool mIsFirstFrame = true;
    bool mHasGLObjects = false;

    int mWindowHeight;
    int mWindowWidth;
};

#endif // __RENDERER.HPP

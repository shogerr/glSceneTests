#ifndef __SCENE_HPP
#define __SCENE_HPP

#include "common.hpp"

class Scene
{
public:
    ~Scene();
    virtual void onStartGraphics();
    virtual void onScreenResize(int width, int height);
    virtual void doFrame();
};

#endif // __SCENE_HPP
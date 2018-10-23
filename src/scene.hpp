#ifndef __SCENE_HPP
#define __SCENE_HPP

#include "common.hpp"

class Scene
{
public:
    ~Scene();
    virtual void OnStartGraphics();
    virtual void OnScreenResize(int width, int height);
    virtual void DoFrame();
};

#endif // __SCENE_HPP
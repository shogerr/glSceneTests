#ifndef __SCENE_HPP
#define __SCENE_HPP

#include "common.hpp"

class Scene
{
public:
    virtual ~Scene();
    virtual void OnStartGraphics();
    virtual void OnScreenResize(int width, int height);
    virtual void DoFrame();
    virtual void OnInstall();
    virtual void OnUnInstall();
    virtual void OnKillGraphics();
    virtual void Poke(unsigned int i);
};

#endif // __SCENE_HPP
#ifndef __SCENE_HPP
#define __SCENE_HPP

#include "common.hpp"

class Scene
{
public:
    virtual ~Scene();
    virtual void OnStartGraphics();
    virtual void OnKillGraphics();

    virtual void OnScreenResize(int width, int height);

    virtual void DoFrame();

    virtual void OnInstall();
    virtual void OnUnInstall();

    virtual void OnMouseMotion(float x, float y);
    virtual void OnJoy(float x, float y);

    virtual void Poke(unsigned int i);
};

#endif // __SCENE_HPP
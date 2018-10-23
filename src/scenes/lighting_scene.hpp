#ifndef __LIGHTING_SCENE_HPP
#define __LIGHTING_SCENE_HPP

#include "../engine_parts.hpp"

class LightingScene : public Scene
{
public:
    virtual void DoFrame();
    virtual void OnStartupGraphics();
private:
    Model* scene_model_;
};

#endif // !__LIGHTING_SCENE_HPP

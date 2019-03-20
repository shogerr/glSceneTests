#ifndef __SCENE_HPP
#define __SCENE_HPP

namespace gl00
{
    class Scene
    {
    public:
        virtual ~Scene() = 0;
        virtual void OnStartGraphics();
        virtual void OnKillGraphics();

        virtual void OnScreenResize(int width, int height);

        virtual void DoFrame();

        virtual void OnInstall();
        virtual void OnUnInstall();

        virtual void OnMouseMotion(float x, float y);
        virtual void OnJoy(float x, float y);

        virtual void Poke(int i);
    };
}
#endif // __SCENE_HPP
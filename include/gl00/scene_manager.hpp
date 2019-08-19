#ifndef __SCENE_MANAGER_HPP
#define __SCENE_MANAGER_HPP

#include <memory>

namespace gl00
{
    class Scene;

    class SceneManager
    {
    public:
        SceneManager();
        ~SceneManager() {};

        SceneManager(SceneManager const&) = delete;
        void operator=(SceneManager const&) = delete;

        void SetScreenSize(int width, int height);
        void KillGraphics();
        void StartGraphics();

        float GetScreenAspect();

        Scene* GetScene();

        void DoFrame();

        void RequestNewScene(std::unique_ptr<Scene> scene);

        void Poke(unsigned int);

        void UpdateMouseMotion(float x, float y);
        void UpdateJoy(float x, float y);

        static SceneManager& GetInstance();
    private:
        std::unique_ptr<Scene> current_scene_;
        int screen_width_, screen_height_;

        std::unique_ptr<Scene> scene_to_install_;
        bool has_graphics_;

        void InstallScene(std::unique_ptr<gl00::Scene> new_scene);
    };
}
#endif // __SCENE_MANAGER_HPP

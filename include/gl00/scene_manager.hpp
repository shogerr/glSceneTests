#ifndef __SCENE_MANAGER_HPP
#define __SCENE_MANAGER_HPP

namespace gl00
{
    class Scene;

    class SceneManager
    {
    public:
        SceneManager();
        ~SceneManager() = default;
        void SetScreenSize(int width, int height);
        void KillGraphics();
        void StartGraphics();

        float GetScreenAspect();

        Scene* GetScene();

        void DoFrame();

        void RequestNewScene(Scene* scene);

        void Poke(unsigned int);

        void UpdateMouseMotion(float x, float y);
        void UpdateJoy(float x, float y);

        static SceneManager* GetInstance();
    private:
        Scene* current_scene_;
        int screen_width_, screen_height_;

        Scene* scene_to_install_;
        bool has_graphics_;

        void InstallScene(gl00::Scene* new_scene);
    };
}
#endif // __SCENE_MANAGER_HPP

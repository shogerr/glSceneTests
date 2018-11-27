#ifndef __SCENE_MANAGER_HPP
#define __SCENE_MANAGER_HPP

class Scene;

class SceneManager
{
public:
    SceneManager();
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

    void InstallScene(Scene* scene);
};
#endif // __SCENE_MANAGER_HPP

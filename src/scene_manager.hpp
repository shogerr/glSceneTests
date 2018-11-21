#ifndef __SCENE_MANAGER_HPP
#define __SCENE_MANAGER_HPP

class Scene;

class SceneManager
{
public:
    SceneManager();
    //~SceneManager();

    static SceneManager* GetInstance();
    void setScreenSize(int width, int height);
    float getScreenAspect();
    Scene* GetScene();
    void InstallScene(Scene* scene);
    void RequestNewScene(Scene* scene);
    void DoFrame();
    void startGraphics();
    void Poke(unsigned int);
    void KillGraphics();

private:
    Scene* mCurScene; 
    Scene* mSceneToInstall;
    bool has_graphics_;
    int mScreenWidth, mScreenHeight;
};
#endif // __SCENE_MANAGER_HPP


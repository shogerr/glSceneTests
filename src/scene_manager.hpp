#ifndef __SCENE_MANAGER_HPP
#define __SCENE_MANAGER_HPP

class Scene;

class SceneManager
{
public:
    SceneManager();
    //~SceneManager();

    static SceneManager* getInstance();
    void setScreenSize(int width, int height);
    float getScreenAspect();
    Scene* getScene();
    void installScene(Scene* scene);
    void RequestNewScene(Scene* scene);
    void doFrame();
    void startGraphics();

private:
    Scene* mCurScene; 
    Scene* mSceneToInstall;
    int mScreenWidth, mScreenHeight;
};
#endif // __SCENE_MANAGER_HPP


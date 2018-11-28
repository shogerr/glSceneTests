#include <scene.hpp>

Scene::~Scene() {}
void Scene::OnStartGraphics() {}
void Scene::OnKillGraphics() {}
void Scene::OnScreenResize(int width, int height) {}
void Scene::DoFrame() {}
void Scene::OnInstall() {}
void Scene::OnUnInstall() {}
void Scene::OnMouseMotion(float x, float y) {}
void Scene::OnJoy(float x, float y) {}
void Scene::Poke(unsigned int) {}

#include <scene_object.hpp>
#include <iostream>

void TestSceneObject()
{
    int* p_i = new int{ 3 };

    gl00::SceneObject<int> i{ p_i };
    std::cout << *i.t_ << std::endl;
}

void main(void)
{
    TestSceneObject();
}
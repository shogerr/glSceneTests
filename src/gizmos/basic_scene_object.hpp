#ifndef __BASIC_SCENE_OBJECT_HPP
#define __BASIC_SCENE_OBJECT_HPP

#include <glm/glm.hpp>

namespace gl00
{

template<typename T>
class BasicSceneObject
{
public:
    BasicSceneObject()
    {
        t = new T();
        pos_ = glm::vec3(0.0);
        v_ = glm::vec3(0.0);
        theta_ = glm::vec3(0.0);
        w_ = glm::vec3(0.0);
    }

    glm::vec3 Theta()
    {
        return theta_;
    }

    void UpdateTheta(float dt)
    {
        theta_ += w_ * dt;

        // Update each component's angle
        for (int i = 0; i < 3; i++)
        {
            if (theta_[i] <= -glm::two_pi<float>())
                theta_[i] += glm::two_pi<float>();
            else if (theta_[i] >= glm::two_pi<float>())
                theta_[i] -= glm::two_pi<float>();
        }
    }

    void SetAngularVelocity(glm::vec3 w) { w_ = w; }

    T* t;

    glm::vec3 pos_;
    glm::vec3 v_;

    glm::vec3 theta_;
    glm::vec3 w_;
};

}
#endif // !__BASIC_SCENE_OBJECT_HPP

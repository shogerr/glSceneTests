#ifndef __SCENE_OBJECT_HPP
#define __SCENE_OBJECT_HPP

#include <glm/glm.hpp>

namespace gl00
{
    class StateSolver
    {
        struct State
        {
            glm::vec3 position;
            glm::vec3 velocity;
            glm::vec3 theta;
            glm::vec3 omega;
        };

        struct Derivative
        {
            glm::vec3 velocity;
            glm::vec3 acceleration;
            glm::vec3 omega;
            glm::vec3 alpha;
        };

    public:
        StateSolver() : force_(glm::vec3(0.0f))
        {
            state_.position = glm::vec3(0.0f);
            state_.velocity = glm::vec3(0.0f);
        }

        void CalcDerivative(const struct State &state, struct Derivative &derivative);
        void Simulate(float dt);

        glm::vec3 force_;
        struct StateSolver::State state_;
    };

    template<typename T>
    class SceneObject
    {
    public:

        SceneObject();
        SceneObject(T* t) : t_(new T(*t)) {}

        void Step(float dt) { solver_.Simulate(dt); }
        glm::vec3 Position() { return solver_.state_.position;  }

        T* t_;
        class StateSolver solver_;
    };
}

#endif // !__SCENE_OBJECT_HPP

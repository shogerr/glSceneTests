#ifndef __SCENE_OBJECT_HPP
#define __SCENE_OBJECT_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>

//#include <common.hpp>

namespace gl00
{
    class StateSolver
    {
    public:
        struct State
        {
            glm::vec3 position;
            glm::vec3 velocity;
            glm::vec3 theta;
            glm::vec3 omega;
            float mass;
            float time;
        };

        struct Derivative
        {
            glm::vec3 velocity;
            glm::vec3 acceleration;
            glm::vec3 omega;
            glm::vec3 alpha;
        };

        StateSolver() : force_(glm::vec3(0.0f))
        {
            state_.position = glm::vec3(0.0f);
            state_.velocity = glm::vec3(0.0f);
        }

        StateSolver(glm::vec3 position) : force_(glm::vec3(0.0f))
        {
            state_.position = position;
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

        SceneObject() : t_(NULL) {}
        SceneObject(const T &t) : t_(t) {}

        void Step(float dt) { solver_.Simulate(dt); }
        glm::vec3 Position() { return solver_.state_.position;  }
        glm::vec3 Position(glm::vec3 position) { solver_.state_.position = position;  }

        T t_;
        class StateSolver solver_;
    };
}

#endif // !__SCENE_OBJECT_HPP

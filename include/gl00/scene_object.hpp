#ifndef __SCENE_OBJECT_HPP
#define __SCENE_OBJECT_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../src/gizmos/state_solver.hpp"

namespace gl00
{
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

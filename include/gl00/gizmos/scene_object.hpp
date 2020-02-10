#ifndef __SCENE_OBJECT_HPP
#define __SCENE_OBJECT_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl00/model.hpp>
#include <gl00/gizmos/state_solver.hpp>

namespace gl00
{
    template<typename T>
    class SceneObject
    {
    public:
        SceneObject(const T& t) : t_(t) {}

        void Step(float dt)
        {
            solver_.Simulate(dt);
            matrix_ = glm::rotate(glm::mat4(1.f), solver_.state_.theta.x, glm::vec3(1.f,0.f,0.f));
            matrix_ = glm::rotate(matrix_, solver_.state_.theta.y, glm::vec3(1.f,0.f,0.f));
            matrix_ = glm::rotate(matrix_, solver_.state_.theta.z, glm::vec3(1.f,0.f,0.f));
            matrix_ = glm::translate(matrix_, solver_.state_.position);
        }

        glm::vec3 Position() { return solver_.state_.position;  }
        glm::vec3 Theta() { return solver_.state_.theta;  }

        void Position(glm::vec3 position) { solver_.state_.position = position;  }
        void Theta(glm::vec3 theta) { solver_.state_.theta = theta;  }

    private:
        const T& t_;
        class StateSolver solver_;
        glm::mat4 matrix_;
    };

/*
    template<>
    class SceneObject<gl00::Model>
    {
        SceneObject(const gl00::Model& t) : t_(t) {}

        void Step(float dt)
        {
            solver_.Simulate(dt);
            //glm::mat4 matrix = glm::rotate(glm::mat4(1.f), solver_.state_.theta.x, glm::vec3(1.f,0.f,0.f));
        }

        glm::vec3 Position() { return solver_.state_.position;  }
        glm::vec3 Theta() { return solver_.state_.theta;  }

        void Position(glm::vec3 position) { solver_.state_.position = position;  }
        void Theta(glm::vec3 theta) { solver_.state_.theta = theta;  }

        const gl00::Model& t_;
        class StateSolver solver_;
    };
*/
}


#endif // !__SCENE_OBJECT_HPP

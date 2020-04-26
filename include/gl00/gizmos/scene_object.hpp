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
        }

        glm::vec3 Position() { return solver_.state_.position;  }
        glm::vec3 Velocity() { return solver_.state_.velocity;  }
        glm::vec3 Theta() { return solver_.state_.theta;  }
        glm::mat4 Matrix()
        {
            //matrix_ = glm::translate(glm::mat4(1.f), -solver_.state_.position);
            auto r_x = glm::vec3(1.f,0.f,0.f)*solver_.state_.theta.x;
            auto r_y = glm::vec3(0.f,1.f,0.f)*solver_.state_.theta.y;
            auto r_z = glm::vec3(0.f,0.f,1.f)*solver_.state_.theta.z;
            auto r = r_x + r_y + r_z;
            matrix_ = glm::mat4(1.f);
            matrix_ = glm::translate(matrix_, solver_.state_.position);
            matrix_ = glm::rotate(matrix_, solver_.state_.theta.z, glm::vec3(0.f,0.f,1.f));
            matrix_ = glm::rotate(matrix_, solver_.state_.theta.y, glm::vec3(0.f,1.f,0.f));
            matrix_ = glm::rotate(matrix_, solver_.state_.theta.x, glm::vec3(1.f,0.f,0.f));
            //matrix_ = glm::rotate(matrix_, glm::length(r), glm::normalize(r));
            //matrix_ = glm::rotate(matrix_, glm::length(solver_.state_.theta), glm::normalize(solver_.state_.theta));
            return matrix_;
        }
        glm::vec3 Force() { return solver_.force_; }
        glm::vec3 ForceAngular() { return solver_.force_angular_; }

        void Force(glm::vec3 force) { solver_.force_ = force; }
        void ForceAngular(glm::vec3 force) { solver_.force_angular_ = force; }
        void Position(glm::vec3 position) { solver_.state_.position = position; }
        void Velocity(glm::vec3 velocity) { solver_.state_.velocity = velocity; }
        void Theta(glm::vec3 theta) { solver_.state_.theta = theta; }

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

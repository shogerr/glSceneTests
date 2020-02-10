#ifndef __STATE_SOLVER_HPP
#define __STATE_SOLVER_HPP

#include <glm/glm.hpp>

namespace gl00
{
    class StateSolver
    {
    public:
        struct State
        {
            glm::vec3 position {0.f, 0.f, 0.f};
            glm::vec3 velocity {0.f, 0.f, 0.f};
            glm::vec3 theta {0.f, 0.f, 0.f};
            glm::vec3 omega {0.f, 0.f, 0.f};
            float mass {0.f};
            float time {0.f};
        };

        struct Derivative
        {
            glm::vec3 velocity;
            glm::vec3 acceleration;
            glm::vec3 omega;
            glm::vec3 alpha;
        };

        StateSolver();
        StateSolver(glm::vec3 position);
        StateSolver(glm::vec3 position, glm::vec3 theta);

        void CalcDerivative(const struct State &state, struct Derivative &derivative);
        void Simulate(float dt);

        glm::vec3 force_;
        glm::vec3 force_angular_;
        struct State state_;
    };
}

#endif // !__STATE_SOLVER_HPP

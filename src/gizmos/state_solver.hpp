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

        StateSolver();
        StateSolver(glm::vec3 position);

        void CalcDerivative(const struct State &state, struct Derivative &derivative);
        void Simulate(float dt);

        glm::vec3 force_;
        struct State state_;
    };
}

#endif // !__STATE_SOLVER_HPP

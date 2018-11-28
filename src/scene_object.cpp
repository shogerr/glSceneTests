#include <scene_object.hpp>

void gl00::StateSolver::CalcDerivative(const State & state, Derivative & derivative)
{
    derivative.velocity = state.velocity;
    derivative.acceleration = force_;
}

void gl00::StateSolver::Simulate(float dt)
{
    Derivative d, d_2;
    State s;

    CalcDerivative(state_, d);
    s.position = state_.position + d.velocity * dt + .5f*d.acceleration*dt*dt;
    s.velocity = state_.velocity + d.acceleration * dt;

    CalcDerivative(s, d_2);
    glm::vec3 aavg = (d.acceleration + d_2.acceleration) * .5f;
    glm::vec3 vavg = (d.velocity + d_2.velocity) * .5f;

    state_.position += vavg * dt;
    state_.velocity += aavg * dt;
}



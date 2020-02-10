#include <gl00/gizmos/state_solver.hpp>

gl00::StateSolver::StateSolver()
    : force_(glm::vec3(0.0f))
    , force_angular_(glm::vec3(0.f))
    , state_(State{})
{}

gl00::StateSolver::StateSolver(glm::vec3 position)
    : force_(glm::vec3(0.0f))
    , force_angular_(glm::vec3(0.0f))
    , state_(State{})
{
    state_.position = position;
}

gl00::StateSolver::StateSolver(glm::vec3 position, glm::vec3 theta)
    : force_(glm::vec3(0.0f))
    , force_angular_(glm::vec3(0.0f))
    , state_(State{})
{
    state_.position = position;
    state_.theta = theta;
}

void gl00::StateSolver::CalcDerivative(const State& state, Derivative& derivative)
{
    derivative.velocity = state.velocity;
    derivative.acceleration = force_;
    derivative.omega = state.omega;
    derivative.alpha = force_angular_;
}

void gl00::StateSolver::Simulate(float dt)
{
    Derivative d, d_2;
    State s;

    CalcDerivative(state_, d);
    s.position = state_.position + d.velocity * dt + .5f*d.acceleration*dt*dt;
    s.velocity = state_.velocity + d.acceleration * dt;

    s.theta = state_.theta + d.omega * dt + .5f * d.omega * dt * dt;
    s.omega = state_.omega + d.alpha * dt;

    CalcDerivative(s, d_2);
    glm::vec3 velocity_avg = (d.velocity + d_2.velocity) * .5f;
    glm::vec3 acceleration_avg = (d.acceleration + d_2.acceleration) * .5f;

    glm::vec3 omega_avg = (d.omega + d_2.omega) * .5f;
    glm::vec3 alpha_avg = (d.alpha + d_2.alpha) * .5f;

    state_.position += velocity_avg * dt;
    state_.velocity += acceleration_avg * dt;

    state_.theta += omega_avg * dt;
    state_.omega += alpha_avg * dt;
}

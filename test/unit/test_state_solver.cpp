#include <gl00/gizmos/state_solver.hpp>
#define BOOST_TEST_MODULE StateSolverTest
#include <boost/test/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE(state_solver_test)
{
    // Set the state's position at 4.9 meters.
    gl00::StateSolver state_solver({0.f,4.9f,0.f}, {220.f,0.f,0.f});

    // Add the force of gravity (-9.8) in the y-direction to the state.
    state_solver.force_ = glm::vec3(0.f,-9.8f,0.f);

    // Add an angular force.
    state_solver.force_angular_ = glm::vec3(0.f,-300.f,0.f);


    // Simulate 1 second.
    //state_solver.Simulate(1.f);

    // Ensure the state's position is 0.
    //BOOST_CHECK(state_solver.state_.position.y == 0.f);

    state_solver.Simulate(.733f);

    std::cout << state_solver.state_.theta.x << std::endl;
    std::cout << state_solver.state_.omega.x << std::endl;

    /*
    gl00::StateSolver::State state {
        {0.f,1.f,0.f},
        {0.f,-9.8f,0.f},
        {0.f,0.0f,0.f},
        {0.f,0.0f,0.f},
        1.f,
        0.f
        };
    */
}

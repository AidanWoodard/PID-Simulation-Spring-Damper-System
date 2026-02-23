/*
This file runs a 1-dimensional simulation of a point moving from point A to
point B with an applied weight (constant).
*/

#include <iostream>
#include "../include/phys_sim.hpp"
#include "../include/PID.hpp"

double PhysicsSim::getPosition() { return currPointPos; }

PhysicsSim::PhysicsSim(PIDCalculator& pidRef) : pid(pidRef) {
    std::cout << "Created new physics simulation." << '\n';
}

void PhysicsSim::beginSimulation(double runTime, double dt) {
    currPointPos = 0.0;
    currPointVel = 0.0;
    elapsedTime = 0.0;
    bool simActive = true;

    while (elapsedTime < runTime) {
        update(dt);

        // safecatch
        if (!simActive) { break; }
        else { elapsedTime += dt; }
    }
}

// FIXME: (Displays var from FileConverter through PID)
void PhysicsSim::test() { pid.sayHello(); }

double PhysicsSim::calculateAccel(double inputForce) {
    // F = M * A   =>   A = F / M
    return (FORCE_GRAVITY + inputForce) / OBJECT_MASS;
}

void PhysicsSim::update(double dt) {
    currPointVel += calculateAccel(appliedForce);
    currPointPos += currPointVel;
    std::cout << currPointPos << std::endl;
}
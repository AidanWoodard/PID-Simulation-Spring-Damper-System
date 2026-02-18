/*
This file runs a 1-dimensional simulation of a point moving from point A to
point B with an applied weight (constant).
*/

#include <iostream>
#include "../include/phys_sim.hpp"

double PhysicsSim::getPosition() { return currPointPos; }

PhysicsSim::PhysicsSim(double activeForce) {
    appliedForce = activeForce;
}

double PhysicsSim::calculateAccel(double appliedForce) {
    return (FORCE_GRAVITY + appliedForce) / OBJECT_MASS;
}

void PhysicsSim::update() {
    currPointVel += calculateAccel(appliedForce);
    currPointPos += currPointVel;
    std::cout << currPointPos << std::endl;
}
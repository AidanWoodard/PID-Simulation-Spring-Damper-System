/*
This file runs a 1-dimensional simulation of a point moving from point A to
point B with an applied weight (constant).
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <format>
#include "../include/phys_sim.hpp"
#include "../include/PID.hpp"
#include "../include/file_conv.hpp"

PhysicsSim::PhysicsSim(PIDCalculator& pidRef, FileConverter& fileRef) 
            : pid(pidRef), fileWriter(fileRef) {}

double PhysicsSim::calculateAccel(double inputForce) {
    // FIXME    A=F/M is a substitute for now
    return (FORCE_GRAVITY + inputForce) / OBJECT_MASS;
}

void PhysicsSim::update() {
    currPointVel += calculateAccel(appliedForce);
    currPointPos += currPointVel;
    fileWriter.recordData(elapsedTime, appliedForce, getPosition(), getVelocity());
    // std::cout << currPointPos << std::endl;
}

void PhysicsSim::beginSimulation(double runTime, double fixed_dt) {
    currPointPos = 0.0;
    currPointVel = 0.0;
    startSimTime = clock();
    bool simActive = true;

    while (elapsedTime < runTime) {
        update();

        // safecatch
        if (!simActive) { break; }
        else { elapsedTime = clock(); }     // FIXME: this is bad practice and inaccurate :(
    }

    // data stored in FileConverter object as 'logBuffer'
    fileWriter.saveSimDataToCSV();

    std::cout << "########################################" << '\n';
    std::cout << std::format("Simulation finished: {:.4f}s", getSimDur()) << '\n';
    std::cout << "########################################" << '\n';
}

double PhysicsSim::getPosition() { return currPointPos; }
double PhysicsSim::getVelocity() { return currPointVel; }

double PhysicsSim::getSimDur() {
    return elapsedTime - startSimTime;
}

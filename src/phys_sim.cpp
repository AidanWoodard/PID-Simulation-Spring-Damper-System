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
    return (inputForce - FORCE_GRAVITY) / OBJECT_MASS;
}

void PhysicsSim::update(double simTime) {
    // calculate applied net force
    double appliedForce = pid.calculateAppliedForce(getPosition(), getVelocity());

    // add to simulation
    currPointVel += calculateAccel(appliedForce);
    currPointPos += currPointVel;
    fileWriter.recordData(simTime, appliedForce, getPosition(), getVelocity());
    // std::cout << currPointPos << std::endl;
}

void PhysicsSim::beginSimulation(int maxRuntimeSeconds) {
    const auto startWallTime = std::chrono::steady_clock::now();
    double simTime = 0.0;
    bool simActive = true;
    int checkIfExceededDur;
    const int EXCEEDED_COUNTER_SET = 100;

    pid.resetPID();

    while (simActive) {
        update(simTime);
        simTime += fixed_dt;

        // killswitch
        if (checkIfExceededDur <= 0) {
            const auto currWallTime = std::chrono::steady_clock::now();
            const std::chrono::duration<double> elapsedSimTime{currWallTime - startWallTime};
            if (elapsedSimTime > std::chrono::duration<double>(maxRuntimeSeconds)) {
                simActive = false;
                fileWriter.saveFinalElapsedTime(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedSimTime).count(), std::chrono::duration_cast<std::chrono::milliseconds>(elapsedSimTime).count());
            } else {
                checkIfExceededDur = EXCEEDED_COUNTER_SET;
            }
        } else { checkIfExceededDur--; }
    }

    // data stored in FileConverter object as 'logBuffer'
    fileWriter.saveSimDataToCSV();

    std::cout << "#############################################" << '\n';
    std::cout << "\tSimulation finished" << '\n';
    std::cout << "#############################################" << '\n';
}

double PhysicsSim::getPosition() { return currPointPos; }
double PhysicsSim::getVelocity() { return currPointVel; }
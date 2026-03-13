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
}

void PhysicsSim::beginSimulation(double maxRuntimeSeconds) {
    const auto startWallTime = std::chrono::steady_clock::now();
    double simTime = 0.0;
    bool simActive = true;
    int checkIfExceededDur = 0;
    const int EXCEEDED_COUNTER_SET = 100;

    pid.resetPID();

    while (simActive) {
        update(simTime);
        simTime += FIXED_DT;

        // simulation clock kill switch
        if (simTime >= KILL_SWITCH_SIM_TIME) {
            // FIXME: not saving final sim time
            //fileWriter.saveFinalElapsedTime(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedWallTime).count(), std::chrono::duration_cast<std::chrono::milliseconds>(elapsedWallTime).count());
            simActive = false;
        }

        // wall clock kill switch
        if (simActive && checkIfExceededDur <= 0) {
            const auto currWallTime = std::chrono::steady_clock::now();
            const std::chrono::duration<double> elapsedWallTime{currWallTime - startWallTime};
            if (std::chrono::duration<double>(maxRuntimeSeconds).count() > 0 && 
                elapsedWallTime > std::chrono::duration<double>(maxRuntimeSeconds)) {
                simActive = false;
                std::cout << std::format("Exiting simulation at {:.6f} elapsed time", std::chrono::duration<double>(maxRuntimeSeconds).count()) << '\n';
                fileWriter.saveFinalElapsedTime(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedWallTime).count(), std::chrono::duration_cast<std::chrono::milliseconds>(elapsedWallTime).count());
            } else {
                std::cout << std::format("Simulation not finished at {}, restarting counter...", elapsedWallTime) << '\n';
                checkIfExceededDur = EXCEEDED_COUNTER_SET;
            }
        } else { checkIfExceededDur--; }
    }

    // data is stored in FileConverter object as 'logBuffer'
    fileWriter.saveSimDataToCSV();

    std::cout << "#############################################" << '\n';
    std::cout << "\tSimulation finished" << '\n';
    std::cout << "#############################################" << '\n';
}

double PhysicsSim::getPosition() { return currPointPos; }
double PhysicsSim::getVelocity() { return currPointVel; }
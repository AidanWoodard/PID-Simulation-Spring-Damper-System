/*
This file runs a 1-dimensional simulation of a point moving from point A to
point B with an applied weight (constant).
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <format>
#include <fstream>
#include "phys_sim.hpp"
#include "PID.hpp"
#include "file_conv.hpp"
#include "sim_config.hpp"

PhysicsSim::PhysicsSim(PIDCalculator& pidRef, FileConverter& fileRef) 
            : pid(pidRef), 
            fileWriter(fileRef) {}

double PhysicsSim::calculateAccel(double inputForce) {
    return (inputForce - GRAV_FORCE) / OBJECT_MASS;
}

void PhysicsSim::update(double simTime) {
    // calculate applied net force
    double appliedForce = pid.calculateAppliedForce(getPosition(), getVelocity(), FIXED_DT);
    currPointVel += calculateAccel(appliedForce) * FIXED_DT;                                    // new v = v + a * dt
    currPointPos += currPointVel * FIXED_DT;                                                    // new p = p + v * dt

    fileWriter.recordData(simTime, appliedForce, getPosition(), getVelocity());
}

std::chrono::duration<double> PhysicsSim::getElapsedTime() {}   //FIXME   

void PhysicsSim::beginSimulation(double maxRuntimeSeconds) {
    const auto startWallTime = std::chrono::steady_clock::now();
    double simTime = 0.0;
    bool simActive = true;
    int checkIfExceededDur = 0;

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
                checkIfExceededDur = TICKS_PER_KILL_SWITCH_CHECK;
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
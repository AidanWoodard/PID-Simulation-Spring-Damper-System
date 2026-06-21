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

double PhysicsSim::calculateAccel(double inputForce) {\
    // F = MA   ->   A = F/M
    return (inputForce - GRAV_FORCE) / OBJECT_MASS;
}

void PhysicsSim::update(double simTime) {
    // calculate applied net force
    double appliedForce = pid.calculateAppliedForce(getPosition(), getVelocity(), FIXED_DT);
    currPointVel += calculateAccel(appliedForce) * FIXED_DT;                                    // new v = v + a * dt
    currPointPos += currPointVel * FIXED_DT;                                                    // new p = p + v * dt

    // save a new data point
    fileWriter.recordData(simTime, appliedForce, getPosition(), getVelocity());
}

std::chrono::duration<double> PhysicsSim::getElapsedTime(auto startTime) {
    // wall time, NOT simulated time (used for killswitch)
    const std::chrono::duration<double> elapsedWallTime{std::chrono::steady_clock::now() - startTime};
    return elapsedWallTime;
}

void PhysicsSim::beginSimulation(bool verboseMode) {
    const auto startWallTime = std::chrono::steady_clock::now();
    double simTime = 0.0;
    bool simActive = true;
    int checkIfExceededDur = 0;

    pid.resetPID();

    while (simActive) {
        update(simTime);
        simTime += FIXED_DT;

        // simulation clock kill switch, calculate using val in json config (under AppState)
        if (simTime >= AppState::config.sim_dur) {
            // calc our wall time currently, save to var 'elapsedWallTime'. Use this for killswitch checks
            const std::chrono::duration<double> elapsedWallTime(getElapsedTime(startWallTime));

            // save final time to fileWriter, to be used to write to .csv
            fileWriter.saveFinalElapsedTime(elapsedWallTime);
            simActive = false;
            if (verboseMode) { std::cout << "DEBUG: kill switch activated by simulated time." << '\n'; }
        }

        // wall clock kill switch. check that our periodic counter is at zero (or ignore if sim stopped)
        if (checkIfExceededDur <= 0 && simActive) {
            const std::chrono::duration<double> elapsedWallTime(getElapsedTime(startWallTime));

            // check that our elapsed time has not exceded the maximum number of seconds permitted
            // in the json config file. this prevents infinite loops or excessive sim times
            if (elapsedWallTime > std::chrono::duration<double>(AppState::config.max_seconds)) {
                simActive = false;
                if (verboseMode) { 
                    std::cout << "DEBUG: kill switch activated by wall time." << '\n';
                    std::cout << std::format("Exiting simulation at {:.6f} elapsed time", std::chrono::duration<double>(AppState::config.max_seconds).count()) << '\n';
                }

                // fileWrite stores this final time for .csv writing of final data
                fileWriter.saveFinalElapsedTime(elapsedWallTime);
            } else {
                std::cout << std::format("Simulation not finished at {}, restarting counter...", elapsedWallTime) << '\n';
                checkIfExceededDur = TICKS_PER_KILL_SWITCH_CHECK;
            }
        } else { checkIfExceededDur--; }
    }

    fileWriter.saveSimDataToCSV();

    std::cout << "#############################################" << '\n';
    std::cout << "\tSimulation finished"<< '\n';
    if (verboseMode) { std::cout << std::setprecision(4) << "Duration: " << getElapsedTime(startWallTime) << " ms" << '\n'; }   // FIXME: not a double or ms
    std::cout << "#############################################" << '\n';
}

double PhysicsSim::getPosition() { return currPointPos; }
double PhysicsSim::getVelocity() { return currPointVel; }
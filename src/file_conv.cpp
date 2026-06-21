/*
This file converts output from the simulation run by main.cpp to a csv 
file format and puts the data into data/telemetry.cpp to be used by the
python visulalization scripts in src/scripts/.

Telemetry.csv is written to in the following format (spaces for clairity):
        ..., timestamp, position, target, ...
*/

#include <iostream>
#include <string>
#include <format>
#include <chrono>
#include <fstream>
#include "file_conv.hpp"
#include "sim_config.hpp"

void FileConverter::recordData(double time, double appliedF, double currPos, double currVel) {
    logBuffer.push_back({time, appliedF, currPos, currVel});
}

void FileConverter::saveSimDataToCSV(bool clearFileBeforeEntry) {
    if (targetFileName == "") {
        std::cerr << "ERROR: No target file for data named. Did you forget to call FileConverter.setTargetFile('file.csv')?" << '\n';
    } else {
        std::fstream TargetFile;
        if (clearFileBeforeEntry) {
            TargetFile.open(targetFileName, std::ios::out | std::ios::trunc);
            std::cout << "Clearing file before entry" << '\n';
        } else {
            TargetFile.open(targetFileName, std::ios::out | std::ios::app);
            std::cout << "Appending to existing file as requested" << '\n';
        }

        TargetFile << std::format("# kp: {} ki: {} kd: {} Duration: {:.4f} ms", 
            AppState::config.kp,
            AppState::config.ki,
            AppState::config.kd,
            elapsedTimeMs) << '\n';

        TargetFile << "Time,Force,Position,Velocity" << '\n';
        for (SimDataPoint dataSnapshot : logBuffer) {
            TargetFile << 
                        dataSnapshot.timeStamp << ',' <<
                        dataSnapshot.currAppliedForce << ',' <<
                        dataSnapshot.currPointPos << ',' <<
                        dataSnapshot.currPointVel << '\n';
        }
        TargetFile.close();
    }
}

void FileConverter::setTargetFile(std::string fileName) {
    targetFileName = fileName;
}

void FileConverter::displayFinalPosData() {
    std::cout << "\nSIMULATION OUPUT: Position" << '\n';
    std::cout << "----------------------------\nTime:\t\tPosition:" << '\n';

    for (SimDataPoint dataSnapshot : logBuffer) {
        std::cout << std::format("{:.4f}\t\t{:.4f}", dataSnapshot.timeStamp, dataSnapshot.currPointPos) << '\n';
    }

    std::cout << "----------------------------\n(End of simulation)" << '\n';
}

void FileConverter::displayFinalVerboseData() {
    std::cout << "\nSIMULATION OUPUT: Position" << '\n';
    std::cout << "----------------------------\nTime:\t\tPosition:\t\tVelocity:\t\tApplied Force:" << '\n';

    for (SimDataPoint dataSnapshot : logBuffer) {
        std::cout << std::format("{:.4f}\t\t{:.4f}\t\t{:.4f}\t\t{:.4f}", 
                dataSnapshot.timeStamp, dataSnapshot.currPointPos, dataSnapshot.currPointVel, dataSnapshot.currAppliedForce) 
                        << '\n';
    }

    std::cout << std::format("----------------------------\nDuration: {:.4f} ms\n(End of simulation)", elapsedTimeMs) << '\n';
}

void FileConverter::saveFinalElapsedTime(std::chrono::duration<double> elapsedWallTime) {
    // convert elapsedWallTime into a more usable double representation in milliseconds
    // create a double_ms so that when we call .count() we don't get a seconds representation
    using double_ms = std::chrono::duration<double, std::milli>;
    double elapsedWallTimeMs = std::chrono::duration_cast<double_ms>(elapsedWallTime).count();

    if (AppState::config.recordSimDuration) {
        if (elapsedWallTimeMs <= 0.0) {
            std::cout << "WARNING: simulation duration is 0.0ms. Make sure that the calls are in the correct order." << '\n';
            elapsedTimeMs = 0.0;
        } else {
            elapsedTimeMs = elapsedWallTimeMs;
        }    
    }
}

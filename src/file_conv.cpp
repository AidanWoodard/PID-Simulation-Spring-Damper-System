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
#include <fstream>
#include "../include/file_conv.hpp"

FileConverter::FileConverter() {};

void FileConverter::recordData(double time, double appliedF, double currPos, double currVel) {
    logBuffer.push_back({time, appliedF, currPos, currVel});
}

void FileConverter::saveSimDataToCSV(bool clearFileBeforeEntry) {
    // TODO: write logBuffer to targetFileName.csv in format {time, applied force, current position} 
    // (which is a SimDataPoint as defined in phys_sim.hpp)
    if (targetFileName == "") {
        std::cerr << "ERROR: No target file for data named. Did you forget to call FileConverter.setTargetFile('file.csv')?" << '\n';
    } else {
        std::fstream TargetFile;
        if (clearFileBeforeEntry) {
            TargetFile.open(targetFileName, std::ios::out | std::ios::trunc);
        } else {
            TargetFile.open(targetFileName, std::ios::out | std::ios::app);
        }
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

void FileConverter::saveFinalElapsedTime(double elapsedTimeNs, double elapsedTimeMs) {
    // FIXME: save as first entry into .csv data
    std::cout << std::format("Simulation duration: {} ns", elapsedTimeNs) << '\n';
    std::cout << std::format("Simulation duration: {:.1f} ms", elapsedTimeMs) << '\n';
}
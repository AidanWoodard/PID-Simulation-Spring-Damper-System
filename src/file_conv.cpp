/*
This file converts output from the simulation run by main.cpp to a csv 
file format and puts the data into data/telemetry.cpp to be used by the
python visulalization scripts in src/scripts/.

Telemetry.csv is written to in the following format (spaces for clairity):
        ..., timestamp, position, target, ...
*/

#include <iostream>
#include <vector>
#include <string>
#include "file_conv.hpp"

FileConverter::FileConverter() {};

void FileConverter::recordData(double time, double appliedF, double currPos, double currVel) {
    logBuffer.push_back({time, appliedF, currPos, currVel});
}

void FileConverter::saveSimDataToCSV() {
    // TODO: write logBuffer to targetFileName.csv in format {time, applied force, current position} 
    // (which is a SimDataPoint as defined in phys_sim.hpp)
}

void FileConverter::setTargetFile(std::string fileName) {
    targetFileName = fileName;
}
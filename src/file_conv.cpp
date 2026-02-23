/*
This file converts output from the simulation run by main.cpp to a csv 
file format and puts the data into data/telemetry.cpp to be used by the
python visulalization scripts in src/scripts/.

Telemetry.csv is written to in the following format (spaces for clairity):
        ..., timestamp, position, target, ...
*/

#include <iostream>
#include <vector>
#include "file_conv.hpp"

FileConverter::FileConverter() {};

void FileConverter::recordData(double time, double appliedF, double currPos) {
    logBuffer.push_back({time, appliedF, currPos});
}

void FileConverter::writeToCSV(std::vector<SimDataPoint>, std::string fileName) {
    // TODO: write to fileName.csv in format {time, applied force, current position} 
    // (which is a SimDataPoint as defined in phys_sim.hpp)
}
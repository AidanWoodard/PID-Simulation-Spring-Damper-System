/*
This file converts output from the simulation run by main.cpp to a csv 
file format and puts the data into data/telemetry.cpp to be used by the
python visulalization scripts in src/scripts/.

Telemetry.csv is written to in the following format (spaces for clairity):
        ..., timestamp, position, target, ...
*/

#include <iostream>
#include <fstream>
#include <format>
#include <vector>

class File_Converter {
    public:
        std::string fileName = "telemetry";
        
        void updateTelemetry(int position) {
            // write to fileName.csv
        }
};
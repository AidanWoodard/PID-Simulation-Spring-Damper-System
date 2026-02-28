/*
This file orchestrates the 1-d physics simluation and applies the math
fucntions written in the PID.cpp file. It also runs file_conv.cpp to 
enter simulation data into telemetry.csv, which is used by python 
visulization scripts in src/python_viz/.
*/

#include "file_conv.hpp"
#include "phys_sim.hpp"
#include "PID.hpp"
#include <iostream>
#include <string>

int main() {
    FileConverter fileConv;
    fileConv.setTargetFile("../data/telemetry.csv");
    PIDCalculator pidCalc(0.0, 0.0, 0.0);
    PhysicsSim physSim(pidCalc, fileConv);            // references, not copies

    // no errors
    return 0;
}
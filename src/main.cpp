/*
This file orchestrates the 1-d physics simluation and applies the math
fucntions written in the PID.cpp file. It also runs file_conv.cpp to 
enter simulation data into telemetry.csv, which is used by python 
visulization scripts in src/python_viz/.
*/

#include "../include/file_conv.hpp"
#include "../include/phys_sim.hpp"
#include "../include/PID.hpp"
#include <iostream>
#include <string>

int main() {
    int maxSimulationSeconds = 0.01;
    double targetPosition = 10.0;
    double kp = 0.0;
    double ki = 0.0;
    double kd = 0.0;

    FileConverter fileConv;
    fileConv.setTargetFile("../data/telemetry.csv");

    PIDCalculator pidCalc(kp, ki, kd);
    pidCalc.setNewTarget(targetPosition);

    PhysicsSim physSim(pidCalc, fileConv);            // references, not copies

    physSim.beginSimulation(maxSimulationSeconds);
    // fileConv.displayFinalPosData();

    // no errors
    return 0;
}
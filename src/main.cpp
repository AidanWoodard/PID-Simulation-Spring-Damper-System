/*
This file orchestrates the 1-d physics simluation and applies the math
fucntions written in the PID.cpp file. It also runs file_conv.cpp to 
enter simulation data into telemetry.csv, which is used by python 
visulization scripts in src/python_viz/.
*/

#include "file_conv.hpp"
#include "phys_sim.hpp"
#include <iostream>

int main() {
    double activeForce = 5.0;

    File_Convert fConv;
    PhysicsSim physSim(activeForce);

    return 0;
}
/*
This file orchestrates the 1-d physics simluation and applies the math
fucntions written in the PID.cpp file. It also runs file_conv.cpp to 
enter simulation data into telemetry.csv, which is used by python 
visulization scripts in src/python_viz/.
*/

#include "file_conv.hpp"         // "../etc" is poor practice, fix when polishing project
#include "phys_sim.hpp"
#include "PID.hpp"
#include "sim_config.hpp"
#include <json.hpp>
#include <iostream>
#include <fstream>
#include <getopt.h>

int main(int argc, char** argv) {
    // Before anything else, load in our essential config data for simulation
    std::ifstream f("../config/sim_config.json");
    if (!f.is_open()) {
        std::cerr << "ERROR WHEN OPENING CONFIG: File not found or otherwise couldn't be opened: " << '\n';
    }
    nlohmann::json config_data;
    f >> config_data;
    AppState::config = SimConfig::from_json(config_data);
    // int i;
    // for (i = 1; i < argc; i++) {
    //     if (argv[i][0] == '-') {
    //         if (argv[i][1] == '-') {
    //             std::cout << "Double flag: " << argv[i] << '\n';
    //         } else {
    //             std::cout << "Single flag: " << argv[i] << '\n';
    //         }
    //     } else if (argv[i][0] != ' ') {
    //         std::cout << "Argument: " << argv[i] << '\n';
    //     }
    // }
    // return 0;
    FileConverter fileConv;
    fileConv.setTargetFile("../data/telemetry.csv");    // FIXME: read from flags in CLI

    double maxSimulationSeconds = 0.8;      // FIXME: add to sim_config.json

    PIDCalculator pidCalc(AppState::config.kp, 
                            AppState::config.ki, 
                            AppState::config.kd);
    pidCalc.setNewTarget(AppState::config.targetPos);

    PhysicsSim physSim(pidCalc, fileConv);            // references, not copies

    physSim.beginSimulation(maxSimulationSeconds);
    //fileConv.displayFinalPosData();

    // no errors
    return 0;
}
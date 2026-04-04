/*
This file orchestrates the 1-d physics simluation and applies the math
fucntions written in the PID.cpp file. It also runs file_conv.cpp to 
enter simulation data into telemetry.csv, which is used by python 
visulization scripts in src/python_viz/.
*/

#include "file_conv.hpp"
#include "phys_sim.hpp"
#include "PID.hpp"
#include "sim_config.hpp"
#include <json.hpp>
#include <iostream>
#include <fstream>
#include <getopt.h>

int main(int argc, char** argv) {
    // Before doing anything, fetch the <config_name>.json config file for this simulation
    // This is given by run.py which handles more complex arg parsing
    std::string config_path;
    std::string csv_target_path;
    int i;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {    // c for config
                config_path = argv[i + 1];
            } else if (argv[i][1] == 't') {  // t for target csv file
                csv_target_path = argv[i + 1];
            } else {
                std::cerr << "\nERROR: Simulation must be run with proper flags and .json passed.\n" << '\n';
                throw std::runtime_error("Could not open .json file and run simulation. Exited simulation.");
            }
        }
    }

    // Open the config and use its namespace
    std::fstream f(config_path);
    if (!f.is_open()) {
        std::cerr << "\nERROR WHEN OPENING CONFIG: File not found or otherwise couldn't be opened.\n" << '\n';
        throw std::runtime_error("Exited simulation.");
    } else {
        nlohmann::json config_data;
        f >> config_data;
        AppState::config = SimConfig::from_json(config_data);
    }
    
    FileConverter fileConv;
    fileConv.setTargetFile(csv_target_path);    // FIXME: read from flags in CLI

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
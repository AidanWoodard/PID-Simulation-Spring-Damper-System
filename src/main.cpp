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
    std::string config_path(argv[1]);
    std::string csv_target_path(argv[2]);
    bool verbose(argv[3][0] == 'T');
    int i;

    // Open the config and use its namespace
    std::fstream f(config_path);
    if (!f.is_open()) {
        std::cerr << "\nERROR WHEN OPENING CONFIG: File not found or otherwise couldn't be opened.\n"
        << "Does the path " << config_path << " exist?" << '\n';
        throw std::runtime_error("Exited simulation.");
    } else {
        nlohmann::json config_data;
        f >> config_data;
        AppState::config = SimConfig::from_json(config_data);
    }
    
    FileConverter fileConv;
    fileConv.setTargetFile(csv_target_path);

    double maxSimulationSeconds = 0.8;      // FIXME: add to sim_config.json

    PIDCalculator pidCalc;
    PhysicsSim physSim(pidCalc, fileConv);            // references, not copies

    physSim.beginSimulation(AppState::config.max_seconds);
    if (verbose) { 
        fileConv.displayFinalPosData();
        std::cout << "(Verbose requested, displayed extra data)" << '\n';
    }

    // no errors
    return 0;
}
/*
This file orchestrates the 1-d physics simluation and applies the math
fucntions written in the PID.cpp file. It also runs file_conv.cpp to 
enter simulation data into telemetry.csv, which is used by python 
visulization scripts in src/python_viz/.
*/

#include "../include/file_conv.hpp"         // this if poor practice, fix when polishing project
#include "../include/phys_sim.hpp"
#include "../include/PID.hpp"
#include "../include/sim_config.hpp"
#include <json.hpp>
#include <iostream>
#include <fstream>

int main() {
    // Before anything else, load in our essential config data for simulation
    std::ifstream f("../config/examples/sharp_pid_config.json");
    nlohmann::json config_data;
    f >> config_data;
    AppState::config = SimConfig::from_json(config_data);

    FileConverter fileConv;
    fileConv.setTargetFile("../data/telemetry.csv");    // FIXME: add file path to sim_config.json

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
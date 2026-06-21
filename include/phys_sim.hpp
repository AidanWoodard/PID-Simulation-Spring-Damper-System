#ifndef PHYS_SIM_HPP
#define PHYS_SIM_HPP

#include <chrono>
#include "sim_config.hpp"

struct SimDataPoint {
    double timeStamp;
    double currAppliedForce;
    double currPointPos;
    double currPointVel;
};

class PIDCalculator;
class FileConverter;

class PhysicsSim {
    private:
    double currPointPos = 0.0;
    double currPointVel = 0.0;

    const double OBJECT_MASS = AppState::config.mass;
    const double GRAV_FORCE = AppState::config.grav_accel * OBJECT_MASS;
    const double FIXED_DT = AppState::config.dt;

    const double KILL_SWITCH_SIM_TIME = AppState::config.sim_dur;   // sim time, not wall time
    constexpr static int TICKS_PER_KILL_SWITCH_CHECK = 200;
    double finalSimDurSec = 0.0;

    PIDCalculator& pid;
    FileConverter& fileWriter;

    double calculateAccel(double inputForce);
    void update(double simTime);
    std::chrono::duration<double> getElapsedTime(auto startTime);
    
    public:
    PhysicsSim(PIDCalculator& pidRef,
                FileConverter& fileWriter);
    void beginSimulation(bool verboseMode = false);
    double getPosition();
    double getVelocity();
};

#endif
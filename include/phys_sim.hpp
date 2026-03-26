#ifndef PHYS_SIM_HPP
#define PHYS_SIM_HPP

#include <chrono>

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

    const double FIXED_DT = 0.01;   // 10 ms, 0.01s
    const double KILL_SWITCH_SIM_TIME = 200.0;   // sim time, not wall time
    const int TICKS_PER_KILL_SWITCH_CHECK = 200;

    PIDCalculator& pid;
    FileConverter& fileWriter;

    static constexpr double OBJECT_MASS = 1.0;      //kg
    static constexpr double FORCE_GRAVITY = 9.81 * OBJECT_MASS;

    double calculateAccel(double inputForce);
    void update(double simTime);
    std::chrono::duration<double> getElapsedTime();
    
    public:
    PhysicsSim(PIDCalculator& pidRef, FileConverter& fileWriter);
    void beginSimulation(double maxRuntimeSeconds);
    double getPosition();
    double getVelocity();
};

#endif
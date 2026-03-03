#ifndef PHYS_SIM_HPP
#define PHYS_SIM_HPP

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

    const double fixed_dt = 0.01;   // 10 ms

    PIDCalculator& pid;
    FileConverter& fileWriter;

    static constexpr double FORCE_GRAVITY = 9.81;
    static constexpr double OBJECT_MASS = 5.0;      //kg

    double calculateAccel(double inputForce);
    void update(double simTime);
    
    public:
    PhysicsSim(PIDCalculator& pidRef, FileConverter& fileWriter);
    void beginSimulation(int maxRuntimeSeconds);
    double getPosition();
    double getVelocity();
};

#endif
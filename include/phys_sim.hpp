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
    double currPointPos;
    double currPointVel;
    double appliedForce;
    bool simActive;

    double elapsedTime;
    double startSimTime;

    PIDCalculator& pid;
    FileConverter& fileWriter;

    static constexpr double FORCE_GRAVITY = -9.81;
    static constexpr double OBJECT_MASS = 5.0;      //kg

    double calculateAccel(double inputForce);
    void update();
    
    public:
    PhysicsSim(PIDCalculator& pidRef, FileConverter& fileWriter);
    void beginSimulation(double runTime, double elapsTime);
    double getPosition();
    double getVelocity();
    double getSimDur();
};

#endif
#ifndef PHYS_SIM_HPP
#define PHYS_SIM_HPP

struct SimDataPoint {
    double timeStamp;
    double currAppliedForce;
    double currPointPos;
};

class PIDCalculator;

class PhysicsSim {
    private:
    double currPointPos;
    double currPointVel;
    double appliedForce;
    bool simActive;

    double elapsedTime;
    double dt;

    PIDCalculator& pid;

    static constexpr double FORCE_GRAVITY = -9.81;
    static constexpr double OBJECT_MASS = 5.0;      //kg

    double calculateAccel(double inputForce);
    void update(double dt);
    double updateDeltaTime(double dt);
    
    public:
    PhysicsSim(PIDCalculator& pidRef);
    void beginSimulation(double runTime, double elapsTime);
    double getPosition();

    // FIXME
    void test();
};

#endif
#ifndef PID_HPP
#define PID_HPP

#include "sim_config.hpp"

class PIDCalculator {
    public:    
    PIDCalculator()
        : kp(AppState::config.kp), 
        ki(AppState::config.ki), 
        kd(AppState::config.kd),
        simTargetPos(AppState::config.targetPos) {}

    void resetPID();
    double calculateAppliedForce(double pos,
                                double vel,
                                double dt);

    void setNewTarget(double pos);

    private:
    double kp;
    double ki;
    double kd;
    double sumError;

    double simTargetPos;
};

#endif
#include <iostream>
#include <fstream>
#include <format>
#include "../include/PID.hpp"
//using namespace std;    // this is poor practice, use std:: for clarity, etc.

PIDCalculator::PIDCalculator(double p, double i, double d)
    : kp(p), ki(i), kd(d) {}

void PIDCalculator::resetPID() {
    // reset the PID by clearing old stored values (not kp, ki, kd)
    sumError = 0.0;
}

double PIDCalculator::calculateAppliedForce(double pos, double vel) {
    // if (pos < simTargetPos && pos > (simTargetPos * 0.75)) {
    //     return 15.0;
    // } else if (pos < simTargetPos) {
    //     return 25.0;
    // } else {
    //     return 0.0;
    // }
    return 10.0;     // FIXME: temporary const
}

void PIDCalculator::setNewTarget(double pos) { simTargetPos = pos; }
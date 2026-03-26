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
    // FIXME: add force barely greater than gravity if not yet at position (testing purposes)
    if (pos < simTargetPos) { return 10.0; }
    else { return 0.0; }
}

void PIDCalculator::setNewTarget(double pos) { simTargetPos = pos; }
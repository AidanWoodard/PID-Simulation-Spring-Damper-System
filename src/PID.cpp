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
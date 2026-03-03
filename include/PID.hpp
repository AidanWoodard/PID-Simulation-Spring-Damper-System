class PIDCalculator {
    public:
    // the "P, I, D" values
    PIDCalculator(double p, 
                double i,
                double d);

    void resetPID();
    double calculateAppliedForce(double pos,
                                double vel);

    void setNewTarget(double pos);

    private:
    double kp;
    double ki;
    double kd;
    double simTargetPos;

    // refactor later. Temporary naming
    double sumError;
};
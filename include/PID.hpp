class PIDCalculator {
    public:
    // the "P, I, D" values
    PIDCalculator(double p, 
                double i,
                double d);

    void sayHello();
    void resetPID();

    private:
    double kp;
    double ki;
    double kd;

    // refactor later. Temporary naming
    double sumError;
};
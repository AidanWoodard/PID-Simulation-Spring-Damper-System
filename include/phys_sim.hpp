class PhysicsSim {
    public:
    PhysicsSim(double activeForce);
    void beginSimulation();
    double getPosition();

    private:
    double currPointPos;
    double currPointVel;
    double appliedForce;
    bool simActive;

    static constexpr double FORCE_GRAVITY = -9.81;
    static constexpr double OBJECT_MASS = 5.0;      //kg

    double calculateAccel(double appliedForce);
    void update();
};
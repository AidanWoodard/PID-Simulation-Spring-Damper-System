#include <vector>
#include "phys_sim.hpp"

class FileConverter {
    public:
    FileConverter();
    void recordData(double time,
                    double appliedF,
                    double currPos);

    void writeToCSV(std::vector<SimDataPoint>);

    private:
    std::vector<SimDataPoint> logBuffer;
};
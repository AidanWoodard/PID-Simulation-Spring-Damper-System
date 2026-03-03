#include <vector>
#include <string>
#include "phys_sim.hpp"

class FileConverter {
    public:
    FileConverter();
    void recordData(double time,
                    double appliedF,
                    double currPos,
                    double currVel);
    void saveSimDataToCSV();
    void setTargetFile(std::string fileName);
    void displayFinalPosData();
    void saveFinalElapsedTime(double elapsedTimeNs, double elapsedTimeMs);

    private:
    std::vector<SimDataPoint> logBuffer;
    std::string targetFileName;
};
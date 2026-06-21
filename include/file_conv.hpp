#ifndef FILE_CONV_HPP
#define FILE_CONV_HPP

#include <vector>
#include <string>
#include <chrono>
#include "phys_sim.hpp"
#include "sim_config.hpp"

class FileConverter {
    public:
    FileConverter() {}
    void recordData(double time,
                    double appliedF,
                    double currPos,
                    double currVel);
    void saveSimDataToCSV(bool clearFileBeforeEntry, bool verbose);
    void setTargetFile(std::string fileName);
    void clearTargetFile(std::string fileName);
    void displayFinalPosData();
    void displayFinalVerboseData();
    void saveFinalElapsedTime(std::chrono::duration<double> elapsedWallTime);

    private:
    std::vector<SimDataPoint> logBuffer;
    std::string targetFileName;
    double elapsedTimeMs = 0.0;
};

#endif
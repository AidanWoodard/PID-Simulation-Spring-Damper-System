#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <json.hpp>

struct SimConfig {
    double grav_accel;
    double dt;
    double max_seconds;
    double mass;
    double startPos;
    double targetPos;
    double kp, ki, kd;
    bool recordSimDuration;

    static SimConfig from_json(const nlohmann::json& json_lib) {
        SimConfig config;
        config.dt           = json_lib["simulation"].value("dt", 0.01);
        config.grav_accel   = json_lib["simulation"].value("gravity_accel", 9.81);
        config.max_seconds  = json_lib["simulation"].value("max_seconds", 0.8);

        config.mass         = json_lib["physical_props"].value("mass_kg", 5.0);
        config.startPos     = json_lib["physical_props"].value("start_pos", 0.0);
        config.targetPos    = json_lib["physical_props"].value("target_pos", 10.0);

        config.kp           = json_lib["pid_gains"].value("kp", 0.0);
        config.ki           = json_lib["pid_gains"].value("ki", 0.0);
        config.kd           = json_lib["pid_gains"].value("kd", 0.0);
    
        return config;
    };
};

namespace AppState {
    inline SimConfig config;
}

#endif
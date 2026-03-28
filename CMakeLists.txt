# required commands
cmake_minimum_required(VERSION 3.14)
project(PID_Simulation)

# make sure we're using the most up to date version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# prepare with its own library
add_library(json_lib INTERFACE)
target_include_directories(json_lib INTERFACE ${CMAKE_SOURCE_DIR}/external)
add_library(nlohmann_json::nlohmann_json ALIAS json_lib)

# create libraries from tool classes
add_library(file_convert    src/file_conv.cpp)
add_library(simulation      src/phys_sim.cpp)
add_library(pid             src/PID.cpp)

# link libraries that use the json parser
target_link_libraries(simulation    PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(pid           PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(file_convert  PUBLIC nlohmann_json::nlohmann_json)

# Create final executable and link libraries
add_executable(new_test src/main.cpp)
target_include_directories(new_test PRIVATE include)
target_link_libraries(new_test
    PUBLIC
        file_convert
        simulation 
        pid 
        nlohmann_json::nlohmann_json
)

# Make sure CMake knows where to look for our sim_config.json/.hpp
target_include_directories(new_test     PRIVATE include)
target_include_directories(simulation   PRIVATE include)
target_include_directories(file_convert PRIVATE include)
target_include_directories(pid          PRIVATE include)
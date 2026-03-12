# required commands
cmake_minimum_required(VERSION 3.5)
project(PID_Simulation)

# make sure we're using the most up to date version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(.)

# create libraries from tool classes
add_library(csv_writer src/file_conv.cpp)
add_library(simulation src/phys_sim.cpp)
add_library(pid src/PID.cpp)

# Create final executable and link libraries
add_executable(new_test src/main.cpp)
target_link_libraries(new_test csv_writer simulation pid)

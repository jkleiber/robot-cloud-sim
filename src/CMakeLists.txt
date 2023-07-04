
project(robot_cloud_sim_lib)
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Compile the core library.
add_subdirectory(core)

# Compile robot/vehicle specific simulators.
add_subdirectory(auv)
add_subdirectory(boat)
add_subdirectory(multirotor)


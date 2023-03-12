
project(robot_cloud_sim_lib)
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Compile the sub-libraries.
add_subdirectory(core)
add_subdirectory(boat)
add_subdirectory(auv)

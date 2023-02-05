# ================= Simulation Binary ==================
add_executable(robot_cloud_sim main.cpp)
target_compile_features(robot_cloud_sim PRIVATE cxx_std_17)
target_link_libraries(robot_cloud_sim PRIVATE cloud_sim_core)

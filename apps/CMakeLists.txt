# ================= Simulation Binary ==================
add_executable(boat_sim boat_main.cpp)
target_compile_features(boat_sim PRIVATE cxx_std_17)
target_link_libraries(boat_sim PRIVATE cloud_sim_core cloud_sim_boat)

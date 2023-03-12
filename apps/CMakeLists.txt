# ================= Simulation Binaries ==================

# Boat
add_executable(boat_sim boat_main.cpp)
target_compile_features(boat_sim PRIVATE cxx_std_17)
target_link_libraries(boat_sim PRIVATE cloud_sim_core cloud_sim_boat)

# AUV
add_executable(auv_sim auv_main.cpp)
target_compile_features(auv_sim PRIVATE cxx_std_17)
target_link_libraries(auv_sim PRIVATE cloud_sim_core cloud_sim_auv)

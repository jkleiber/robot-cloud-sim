# ================= Simulation Binaries ==================

# AUV
add_executable(auv_sim auv_main.cpp)
target_compile_features(auv_sim PRIVATE cxx_std_17)
target_link_libraries(auv_sim PRIVATE cloud_sim_core cloud_sim_auv)

# Boat
add_executable(boat_sim boat_main.cpp)
target_compile_features(boat_sim PRIVATE cxx_std_17)
target_link_libraries(boat_sim PRIVATE cloud_sim_core cloud_sim_boat)

# Multirotor
add_executable(multirotor_sim multirotor_main.cpp)
target_compile_features(multirotor_sim PRIVATE cxx_std_17)
target_link_libraries(multirotor_sim PRIVATE cloud_sim_core cloud_sim_multirotor)

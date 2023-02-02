cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

file(GLOB HEADER_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/include/core/*.h")

file(GLOB SOURCE_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/src/core/*.cpp")

# Create the control simulation library
add_library(cloud_sim_core ${SOURCE_LIST} ${HEADER_LIST})
set_target_properties(cloud_sim_core PROPERTIES LINKER_LANGUAGE CXX)

if(WIN32)
    get_target_property(EIGEN_INCLUDES Eigen3::Eigen INCLUDE_DIRECTORIES)
    target_include_directories(cloud_sim_core PUBLIC ../../include ${EIGEN_INCLUDES})
else()
    target_include_directories(cloud_sim_core PUBLIC ../../include)
endif(WIN32)

# This depends on Eigen
target_link_libraries(cloud_sim_core PUBLIC Eigen3::Eigen)

# All users of this library will need at least C++17
target_compile_features(cloud_sim_core PUBLIC cxx_std_17)

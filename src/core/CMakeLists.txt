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

# Find required packages.
# find_package(Crow CONFIG REQUIRED)
# find_package(Eigen3 CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Target all library dependencies
target_link_libraries(cloud_sim_core
    PUBLIC
    gRPC::grpc++
)

# Target the include folder
target_include_directories(cloud_sim_core PUBLIC ../../include)

# All users of this library will need at least C++17
target_compile_features(cloud_sim_core PUBLIC cxx_std_17)

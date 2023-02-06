cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

file(GLOB HEADER_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/include/boat/*.h")

file(GLOB SOURCE_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/src/boat/*.cpp")

# Create the control simulation library
add_library(cloud_sim_boat ${SOURCE_LIST} ${HEADER_LIST})
set_target_properties(cloud_sim_boat PROPERTIES LINKER_LANGUAGE CXX)

# Find required packages.
find_package(asio CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

# Target all library dependencies
target_link_libraries(cloud_sim_boat PUBLIC Eigen3::Eigen asio::asio Crow::Crow)

# Target the include folder
target_include_directories(cloud_sim_boat PUBLIC ../../include)

# All users of this library will need at least C++17
target_compile_features(cloud_sim_boat PUBLIC cxx_std_17)

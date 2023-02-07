cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

file(GLOB HEADER_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/include/boat/*.h")

file(GLOB SOURCE_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/src/boat/*.cpp")

list(APPEND SOURCE_LIST ${PROTO_SRC})
list(APPEND HEADER_LIST ${PROTO_HEADER})

# Create the control simulation library
add_library(cloud_sim_boat ${SOURCE_LIST} ${HEADER_LIST})
set_target_properties(cloud_sim_boat PROPERTIES LINKER_LANGUAGE CXX)

# Find required packages.
find_package(asio CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

message(STATUS "foo include dir: ${Protobuf_LIBRARIES}")

# Target all library dependencies
target_link_libraries(cloud_sim_boat
    PUBLIC
    Eigen3::Eigen
    asio::asio
    Crow::Crow
    rapidjson
    protobuf::libprotoc
    protobuf::libprotobuf
    protobuf::libprotobuf-lite
    cloud_sim_msg
)

# Target the include folder
target_include_directories(cloud_sim_boat PUBLIC ../../include ${Protobuf_INCLUDE_DIRS} ${CMAKE_BINARY_DIR}/src/gen)

# All users of this library will need at least C++17
target_compile_features(cloud_sim_boat PUBLIC cxx_std_17)

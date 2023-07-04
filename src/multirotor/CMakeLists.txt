cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

file(GLOB HEADER_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/include/multirotor/*.h")

file(GLOB SOURCE_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/src/multirotor/*.cpp")

# Generate protobuf messages
file(GLOB PROTO_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/src/multirotor/*.proto")

# We need protobuf and gRPC to generate the protobuf messages for communication.
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Generate protobuf for the C++ code
# protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTO_LIST}) # PLUGIN protoc-gen-grpc=${GRPC_CPP_PLUGIN_PATH})

# Generate gRPC stuff specifically
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
# protobuf_generate(PROTOS ${PROTO_LIST} OUT_VAR GRPC_PROTO_SRCS LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

list(APPEND SOURCE_LIST ${PROTO_SRC})
list(APPEND HEADER_LIST ${PROTO_HEADER})

# Create the control simulation library
add_library(cloud_sim_multirotor ${SOURCE_LIST} ${HEADER_LIST} ${GRPC_PROTO_SRCS})
set_target_properties(cloud_sim_multirotor PROPERTIES LINKER_LANGUAGE CXX)

# Find required packages.
find_package(Crow CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

# message(STATUS "foo include dir: ${Protobuf_LIBRARIES}")

# Target all library dependencies
target_link_libraries(cloud_sim_multirotor
    PUBLIC
    cloud_sim_core
    Eigen3::Eigen
    Crow::Crow
    protobuf::libprotoc
    protobuf::libprotobuf
    protobuf::libprotobuf-lite
    gRPC::grpc
    gRPC::grpc++
)

# Target the include folder
target_include_directories(cloud_sim_multirotor PUBLIC ../../include ${Protobuf_INCLUDE_DIRS} ${CMAKE_BINARY_DIR}/src)

# All users of this library will need at least C++17
target_compile_features(cloud_sim_multirotor PUBLIC cxx_std_17)

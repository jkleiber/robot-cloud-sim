cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

include(${RobotCloudSim_SOURCE_DIR}/src/msg/myprotobuf.cmake)

# Generate protobuf messages
file(GLOB PROTO_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/src/msg/*.proto")

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTO_LIST})
protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTO_LIST} PLUGIN protoc-gen-grpc=${GRPC_CPP_PLUGIN_PATH})

# Generate gRPC stuff specifically
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(PROTOS ${PROTO_LIST} OUT_VAR GRPC_PROTO_SRCS LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

# my_protobuf_generate_cpp(../gen/msg PROTO_SRC PROTO_HEADER ${PROTO_LIST})

# protobuf_generate(TARGET myproto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")
message(STATUS "GRPC SRCS: ${GRPC_PROTO_SRCS}")

# Create the control simulation library
add_library(cloud_sim_msg ${PROTO_SRC} ${PROTO_HEADER} ${GRPC_PROTO_SRCS})
set_target_properties(cloud_sim_msg PROPERTIES LINKER_LANGUAGE CXX)

# Target all library dependencies
target_link_libraries(cloud_sim_msg
    PUBLIC
    protobuf::libprotoc
    protobuf::libprotobuf
    protobuf::libprotobuf-lite
    gRPC::grpc
    gRPC::grpc++
)

# Target the include folder
target_include_directories(cloud_sim_msg PUBLIC ../../include ${Protobuf_INCLUDE_DIRS})

# All users of this library will need at least C++17
target_compile_features(cloud_sim_msg PUBLIC cxx_std_17)

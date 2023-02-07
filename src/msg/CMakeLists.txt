cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Generate protobuf messages
file(GLOB PROTO_LIST
    CONFIGURE_DEPENDS
    "${RobotCloudSim_SOURCE_DIR}/src/msg/*.proto")

find_package(Protobuf CONFIG REQUIRED)
protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTO_LIST})

# Create the control simulation library
add_library(cloud_sim_msg ${PROTO_SRC} ${PROTO_HEADER})
set_target_properties(cloud_sim_msg PROPERTIES LINKER_LANGUAGE CXX)

# Target all library dependencies
target_link_libraries(cloud_sim_msg
    PUBLIC
    ${Protobuf_LIBRARIES}
)

# Target the include folder
target_include_directories(cloud_sim_msg PUBLIC ../../include)

# All users of this library will need at least C++17
target_compile_features(cloud_sim_msg PUBLIC cxx_std_17)

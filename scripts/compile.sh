#! /bin/bash

# Find compile script working directory
cd "$(dirname "$0")"
SCRIPTS_DIR="$(pwd)"
COMPILE_WD="$(pwd)/../build"
APP_DIR="$(pwd)/../build/apps"
CONFIG_WD="$(pwd)/../config"
TEMPLATES_WD="$(pwd)/../templates"
VCPKG_PATH="$(pwd)/../vcpkg"
PROTO_DIR="$(pwd)/../src/boat"

# Use vcpkg toolchain
cmake -B $COMPILE_WD -S .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_PATH/scripts/buildsystems/vcpkg.cmake
if [ $? -ne 0 ]
then
    echo "Error: CMake vcpkg failed!"
    exit 1
fi

# CMake the code, exit on failure
cmake --build $COMPILE_WD 
if [ $? -ne 0 ]
then
    echo "Error: CMake build failed!"
    exit 2
fi

# Copy the static files (resources, protobufs, etc.) to the binary directory.
./cp_static

echo "Compiled successfully!"

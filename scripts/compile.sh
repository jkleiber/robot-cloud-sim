#! /bin/bash

# Find compile script working directory
cd "$(dirname "$0")"
SCRIPTS_DIR="$(pwd)"
COMPILE_WD="$(pwd)/../build"
CONFIG_WD="$(pwd)/../config"

# TODO: make this configurable
VCPKG_PATH="/opt/vcpkg"

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

# Copy the current configuration to the binary location, exit on failure
cd $COMPILE_WD
rm -rf $COMPILE_WD/config
cp -R $CONFIG_WD $COMPILE_WD
if [ $? -ne 0 ]
then
    echo "Error: Failed to copy configuration files"
    exit 3
fi

echo "Compiled successfully!"

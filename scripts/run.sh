#! /bin/bash

# Change to scripts directory and run relative to it
cd "$(dirname "$0")"
APP_BIN="$(pwd)/../build/apps"


# Change to the robot binaries directory and run the simulator code
cd ${APP_BIN}
./boat_sim
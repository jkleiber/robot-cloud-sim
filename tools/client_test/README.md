
# client_test

This is a test python script for communicating with the simulator.

## Dependencies

This uses pipenv for package management, so you should install that first.

## Setup

1. Run the `build_protos.sh` (or `wbuild_protos.bat`) script to generate the python files from the protocol buffers specified in the `msg` package on the C++ side of the codebase.
2. Compile and run the simulator
3. Run `pipenv run python main.py` to rename the boat and show that the gRPC communication works

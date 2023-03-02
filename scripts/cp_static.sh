#! /bin/bash

# Change to the cp_static.sh directory
cd "$(dirname "$0")"
APP_DIR="$(pwd)/../build/apps"
TEMPLATES_WD="$(pwd)/../server/templates"
STATIC_DIR="$(pwd)/../server/static"
PROTO_DIR="$(pwd)/../src/boat"

# Copy the templates to the binary location, exit on failure
cd $APP_DIR
rm -rf $APP_DIR/templates
cp -R $TEMPLATES_WD $APP_DIR
if [ $? -ne 0 ]
then
    echo "Error: Failed to copy templates"
    exit 3
fi

# Copy the protobuf messages to the binary location, exit on failure
cd $APP_DIR
mkdir -p $APP_DIR/static/boat/ 
cp -R $PROTO_DIR/*.proto $APP_DIR/static/boat/ 
if [ $? -ne 0 ]
then
    echo "Error: Failed to copy protocols"
    exit 4
fi

# Copy the boat sim's static files, exit on failure
cp -R $STATIC_DIR/boat/* $APP_DIR/static/boat/
if [ $? -ne 0 ]
then
    echo "Error: Failed to copy boat simulator static files"
    exit 5
fi
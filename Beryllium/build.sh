#!/bin/bash

# Ensure we are in the project directory
cd "$(dirname "$0")"

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Run CMake
cmake ..

# Build the project
cmake --build .

./Beryllium
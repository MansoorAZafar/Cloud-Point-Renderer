#!/bin/bash
BUILD_DIRECTORY="build/"

echo "!!! Building Application if not built already !!!"
if [ ! -d "$BUILD_DIRECTORY" ]; then
    echo "Application not built, building application with CMake"
    mkdir build/
    cd build/
    cmake ..
    cmake --build .
    cd ../
fi

echo "!!! Success !!!"
#!/bin/bash

DENSITY=0
RESOURCE="resources/trophy.obj"
if [ ! -z "$1" ]; then
    RESOURCE="$1"
fi

if [ ! -z "$2" ]; then
    DENSITY="$2"
fi


./build.sh

echo "Running Application with default "$RESOURCE" "
./build/bin/Debug/CloudPointRenderer.exe "$RESOURCE" "$DENSITY" || echo "Resource not found or improper"


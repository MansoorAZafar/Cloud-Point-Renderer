#!/bin/bash

RESOURCE="resources/trophy.obj"
if [ ! -z "$1" ]; then
    RESOURCE="$1"
fi

./build.sh

echo "Running Application with default "$RESOURCE" "
./build/bin/Debug/CloudPointRenderer.exe "$RESOURCE" || echo "Resource not found or improper"


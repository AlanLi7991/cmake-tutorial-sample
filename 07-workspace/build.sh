#!/bin/sh
set -o errexit

rm -rf build
mkdir build
cd build
cmake ../.
cmake --build .  --target main
./main/main

set +o errexit
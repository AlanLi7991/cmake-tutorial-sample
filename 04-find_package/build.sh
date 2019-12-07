#!/bin/sh

set -o errexit

rm -rf build
mkdir build
cd build
cmake ../.
cmake --build .
./find_opencv ../lena.jpg

set +o errexit
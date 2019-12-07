#!/bin/sh

set -o errexit

rm -rf build
mkdir build
cd build

#Genrate build folder
cmake ../.
#make, execute compile command
make
#run
./main

set +o errexit
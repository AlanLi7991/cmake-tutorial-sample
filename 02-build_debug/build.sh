#!/bin/sh

set -o errexit

function build_with_type()
{
    rm -rf build_$1
    mkdir build_$1
    cd build_$1
    cmake ../. -DCMAKE_BUILD_TYPE=$1
    cmake --build .
    ./main
}

build_with_type "Debug" 
echo  "---------------------------------"
cd -
build_with_type "Release"

set +o errexit
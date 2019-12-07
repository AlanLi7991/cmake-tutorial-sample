#!/bin/sh
set -o errexit

rm -rf build
mkdir build
cd build
cmake ../. -D CMAKE_SYSTEM_NAME=iOS
rm -rf ./*
cmake ../. -D CMAKE_SYSTEM_NAME=Apple

set +o errexit
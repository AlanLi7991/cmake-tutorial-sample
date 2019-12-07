#!/bin/sh
set -o errexit

rm -rf build
mkdir build
cd build
cmake ../.

set +o errexit

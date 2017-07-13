#!/bin/bash

set -e

mkdir -p build/
cd build/
rm -rf CMake*
cmake .. -G"MSYS Makefiles"
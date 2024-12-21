#!/bin/bash
CORES=1
if [[ $OSTYPE == 'darwin'* ]]; then
    CORES=$(sysctl -n hw.logicalcpu)
else
    CORES=$(nproc)
fi
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j${CORES}
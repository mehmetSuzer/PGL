#!/bin/bash

if [ -d "./build" ]; then
    rm -r build
fi

mkdir build
cmake -B build -S .
cmake --build build --config Release -j ${nproc}

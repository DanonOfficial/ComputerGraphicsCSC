#!/usr/bin/env bash
if [ ! -d "build" ]; then
    mkdir build
fi
if [ ! -d "bin" ]; then
    mkdir bin
fi
cd build
cmake ..
cd ..
cmake --build ./build


cp build/Task2 bin/Task2
cp -r shaders bin/shaders
cp -r objects bin/objects
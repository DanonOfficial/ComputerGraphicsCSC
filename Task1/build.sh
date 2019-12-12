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


cp build/Task1 bin/Task1
cp imgui.ini bin/imgui.ini
cp imgui_lib.ini bin/imgui_lib.ini
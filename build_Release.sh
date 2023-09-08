#!/bin/bash

cmake -S . -B build/Release  -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release

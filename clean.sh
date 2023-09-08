#!/bin/bash

cmake --build build/Debug --target clean
cmake --build build/Release --target clean
rm -rf build

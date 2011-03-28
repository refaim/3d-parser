#!/bin/sh
mkdir build
cd build
cmake -G "Unix Makefiles" -DBUILD_ASSIMP_TOOLS=OFF .. && make
cd ..

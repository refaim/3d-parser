@echo off
pushd %~dp0
mkdir build 2>nul
pushd build
cmake -G "Unix Makefiles" -DBUILD_ASSIMP_TOOLS=OFF .. && make %* && popd && popd

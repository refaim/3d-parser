@echo off
pushd build
cmake -G "Unix Makefiles" -DENABLE_BOOST_WORKAROUND=ON -DBUILD_ASSIMP_TOOLS=OFF .. && make
popd

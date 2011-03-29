.PHONY: clean

CMAKE_GENERATOR = "Unix Makefiles"
CMAKE_FLAGS = -DBUILD_ASSIMP_TOOLS=OFF

all: executable

debug: CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=Debug
debug: executable

executable:
	-mkdir build
	cd build && cmake -G $(CMAKE_GENERATOR) $(CMAKE_FLAGS) .. && make

clean:
	-rm -rf build bin lib
	-del /s /q build bin lib

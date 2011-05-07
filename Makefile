.PHONY: clean

BUILDER =
CONFIGURATION = Debug
CMAKE_FLAGS = -DBUILD_ASSIMP_TOOLS=OFF

all: mingw executable

# configurations
debug: CONFIGURATION = Debug
debug: executable

release: CONFIGURATION = Release
release: executable

# result types
gnumake: CMAKE_GENERATOR = "Unix Makefiles"
gnumake: BUILDER = && make
gnumake: executable

mingw: CMAKE_GENERATOR = "MinGW Makefiles"
mingw: BUILDER = && mingw32-make
mingw: executable

nmake: CMAKE_GENERATOR = "NMake Makefiles"
nmake: BUILDER = && nmake
nmake: executable

vs2008: CMAKE_GENERATOR = "Visual Studio 9 2008"
vs2008: executable

# targets
executable:
	-mkdir -p build
	+cd build && cmake -G $(CMAKE_GENERATOR) $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=$(CONFIGURATION) .. $(BUILDER)

clean:
	-rm -rf build bin lib
	-del /s /q build bin lib

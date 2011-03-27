# define the sources
set (VIEWER_SRC viewer.cpp)

ADD_DEFINITIONS (-std=c++98 -W -Wall -pedantic -Wextra -Weffc++
                 -Woverloaded-virtual -Wnon-virtual-dtor
                 -Wold-style-cast -Wconversion -Wsign-conversion
                 -Winit-self -Wunreachable-code
                )

# define executable
ADD_EXECUTABLE (viewer ${VIEWER_SRC})

TARGET_LINK_LIBRARIES (viewer assimp)
ADD_DEPENDENCIES (viewer assimp)

# install binaries
INSTALL (TARGETS viewer RUNTIME DESTINATION bin)
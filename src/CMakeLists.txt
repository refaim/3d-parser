# define the sources
set (VIEWER_SRC viewer.cpp)

# define executable
ADD_EXECUTABLE (viewer ${VIEWER_SRC})

# libraries
ADD_DEPENDENCIES (viewer assimp)
TARGET_LINK_LIBRARIES (viewer assimp)
TARGET_LINK_LIBRARIES (viewer ${QT_LIBRARIES})

# install binaries
INSTALL (TARGETS viewer RUNTIME DESTINATION bin)

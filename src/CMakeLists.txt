# prepare QT
SET (viewer_SOURCES viewer.cpp main.cpp mainwindow.cpp scenemodel.cpp)
SET (viewer_HEADERS viewer.h mainwindow.h scenemodel.h)
SET (viewer_FORMS mainwindow.ui)
QT4_WRAP_CPP(viewer_HEADERS_MOC ${viewer_HEADERS})
QT4_WRAP_UI(viewer_FORMS_HEADERS ${viewer_FORMS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
SET(QT_USE_QTOPENGL TRUE)


# define executable
ADD_EXECUTABLE (viewer WIN32 ${viewer_SOURCES} ${viewer_HEADERS_MOC} ${viewer_FORMS_HEADERS})

# libraries
ADD_DEPENDENCIES (viewer assimp)
TARGET_LINK_LIBRARIES (viewer assimp)
TARGET_LINK_LIBRARIES (viewer ${QT_LIBRARIES})
FIND_PACKAGE(OpenGL REQUIRED)
TARGET_LINK_LIBRARIES (viewer ${OPENGL_gl_LIBRARY})

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# install binaries
INSTALL (TARGETS viewer RUNTIME DESTINATION bin)

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (viewer)

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# external libraries
## Qt
SET (QTLIBS QtCore QtGui)
FIND_PACKAGE (Qt4 COMPONENTS ${QTLIBS} REQUIRED)
INCLUDE(${QT_USE_FILE})
### ignore warnings from Qt headers
INCLUDE_DIRECTORIES (SYSTEM ${QT_INCLUDES})

IF (WIN32)
    ### copy Qt libraries
    FOREACH (lib ${QTLIBS})
        SET (DLLNAME ${lib}${TYPE}4.dll)
        ADD_CUSTOM_TARGET (${lib}_copy ALL
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${QT_BINARY_DIR}/${DLLNAME}
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            COMMENT "Copying ${DLLNAME} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        )
    ENDFOREACH (lib)
ENDIF (WIN32)

## assimp
ADD_SUBDIRECTORY (contrib/assimp assimp)
INCLUDE_DIRECTORIES (SYSTEM contrib/assimp/include)

# compiler flags
SET (GCC_FLAGS -std=c++98 -W -Wall -pedantic -Wextra -Weffc++
               -Woverloaded-virtual -Wnon-virtual-dtor
               -Wold-style-cast -Wconversion -Wsign-conversion
               -Winit-self -Wunreachable-code
    )

IF (CMAKE_COMPILER_IS_GNUCC)
    SET (ADDITIONAL_CXX_FLAGS ${GCC_FLAGS})
ELSE (CMAKE_COMPILER_IS_GNUCC)
    SET (ADDITIONAL_CXX_FLAGS "")
ENDIF (CMAKE_COMPILER_IS_GNUCC)

FOREACH (flag ${ADDITIONAL_CXX_FLAGS})
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
ENDFOREACH (flag)

# main project
ADD_SUBDIRECTORY (src viewer)

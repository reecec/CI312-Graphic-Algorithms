CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Geometry)

#############################################################################
# find sources

# we glob for sources, you can also just list them here as is traditional for CMake
FILE(GLOB SRC_FILES "Geometry.cpp")

#############################################################################
# build executable

SET(EXE ${PROJECT_NAME})

# add include diretories for OpenSG and dependency headers
INCLUDE_DIRECTORIES(${OpenSG_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${DEP_INCLUDE_DIRS})

# add library search paths for OpenSG and depdency headers
LINK_DIRECTORIES(${OpenSG_LIBRARY_DIRS})
LINK_DIRECTORIES(${DEP_LIBRARY_DIRS})



ADD_EXECUTABLE(${EXE} ${SRC_FILES})
message(${OpenSG_LIBRARIES})

#message(${DEP_LIBRARIES})
# link against libraries
TARGET_LINK_LIBRARIES(${EXE} ${OpenSG_LIBRARIES})
TARGET_LINK_LIBRARIES(${EXE} debug osgglutD)
TARGET_LINK_LIBRARIES(${EXE} optimized osgglut)
TARGET_LINK_LIBRARIES(${EXE} debug libboost_system-vc140-mt-gd-1_62.lib)
TARGET_LINK_LIBRARIES(${EXE} optimized libboost_system-vc140-mt-1_62.lib)
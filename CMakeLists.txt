cmake_minimum_required(VERSION 2.8)
project(CI312Tutorials)

###################################
#find required packages

set(Boost_DEBUG 1)
# set this to where the FindOpenSG.cmake module is
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "C:/OpenSG/share/OpenSG/cmake")

#############################################################################
# get dependencies
SET(OpenSG_ROOT		 "C:/OpenSG")


# OSG_COMPONENTS holds all OpenSG libraries you want to use
SET(OSG_COMPONENTS OSGBase OSGCluster OSGDrawable OSGFileIO OSGGroup OSGState OSGSystem OSGUtil OSGWindow OSGWindowGLUT)

# These variables collect all dependencies for the application
SET(DEP_INCLUDE_DIRS)
SET(DEP_LIBRARY_DIRS)
SET(DEP_LIBRARIES)

LIST(APPEND DEP_LIBRARY_DIRS "C:/OpenSG.Support/lib")


#############
#### Collada (optional, depending on how OpenSG was built)

FIND_PACKAGE(Collada)

IF(COLLADA_FOUND)
LIST(APPEND DEP_INCLUDE_DIRS ${COLLADA_INCLUDE_DIRS})
LIST(APPEND DEP_LIBRARIES ${COLLADA_LIBRARIES})
ENDIF(COLLADA_FOUND)

#############
#### Boost
SET(BOOST_ROOT		 "C:/OpenSG.Support")

SET(Boost_USE_MULTITHREAD ON)
SET(Boost_STATIC_LIBS OFF)

IF(COLLADA_FOUND)
#SET(Boost_COMPONENTS filesystem)
FIND_PACKAGE(Boost COMPONENTS ${Boost_COMPONENTS} REQUIRED)
ELSE(COLLADA_FOUND)
FIND_PACKAGE(Boost REQUIRED)
ENDIF(COLLADA_FOUND)

FIND_PACKAGE(Boost COMPONENTS ${Boost_COMPONENTS} REQUIRED)

# with boost 1.35.0 and newer boost filesystem depends on boost system
#IF(Boost_FOUND AND Boost_MINOR_VERSION GREATER "34")
#SET(Boost_COMPONENTS system ${Boost_COMPONENTS})

#FIND_PACKAGE(Boost COMPONENTS ${Boost_COMPONENTS} REQUIRED)
#ENDIF(Boost_FOUND AND Boost_MINOR_VERSION GREATER "34")

IF(Boost_FOUND)
LIST(APPEND DEP_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
#LIST(APPEND DEP_LIBRARIES ${Boost_LIBRARIES})
ENDIF(Boost_FOUND)

LIST(APPEND DEP_LIBRARY_DIRS "${BOOST_ROOT}/lib")


#############
#### GLUT
SET(GLUT_ROOT		 "C:/OpenSG.Support")
SET(GLUT_INCLUDE_DIR		 "C:/OpenSG.Support/include")
SET(GLUT_glut_LIBRARY		 "C:/OpenSG.Support/lib")

FIND_PACKAGE(GLUT REQUIRED)
IF(GLUT_FOUND)
  LIST(APPEND DEP_INCLUDE_DIRS ${GLUT_INCLUDE_DIR})
  LIST(APPEND GLUT_LIBRARIES    "${GLUT_LIBRARIES}")
  LIST(APPEND GLUT_LIBRARIES_DEBUG   "${GLUT_LIBRARIES}")

ELSE()

	LIST(APPEND DEP_INCLUDE_DIRS "${GLUT_ROOT}/include")
	LIST(APPEND GLUT_LIBRARIES    "${GLUT_ROOT}/lib")
	LIST(APPEND GLUT_LIBRARIES_DEBUG   "${GLUT_ROOT}")
ENDIF()


#############
#### OpenSG

FIND_PACKAGE(OpenSG REQUIRED COMPONENTS ${OSG_COMPONENTS})

###EXCLUDE freeglut

SET(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"freeglut.lib\")

if(MSVC)
  # Force to always compile with W1
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W1" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W1")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

message("libraries of opensg " ${OpenSG_LIBRARIES})
message("libraries dirs of opensg " ${OpenSG_LIBRARY_DIRS})


###################################
#### add subdirectories
add_subdirectory(SimpleExample)
add_subdirectory(Geometry)
add_subdirectory(Polyhedra)
add_subdirectory(DualTetrahedron)
add_subdirectory(DataStructures)
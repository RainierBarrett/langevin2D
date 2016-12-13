# Rainier Barrett. Copyright (C) 206. GPLv3


#
# Project Properties
#
cmake_minimum_required (VERSION 2.8)
project (langevin2D)
set (APPLICATION_NAME "langevin2D")
set (APPLICATION_CODENAME "${PROJECT_NAME}")
set (APPLICATION_COPYRIGHT_YEARS "2016")
set (APPLICATION_VERSION_MAJOR 0)
set (APPLICATION_VERSION_MINOR 1)
set (APPLICATION_VERSION_PATCH 0)
set (APPLICATION_VERSION_TYPE SNAPSHOT)
set (APPLICATION_VERSION_STRING "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}-${APPLICATION_VERSION_TYPE}")
#set (APPLICATION_VENDOR_ID "com.yourcompany")
#set (APPLICATION_VENDOR_NAME "Your Company")
#set (APPLICATION_VENDOR_URL "yourcompany.com")
#set (APPLICATION_ID "${APPLICATION_VENDOR_ID}.${PROJECT_NAME}")


#
# Debugging Options
#
set (CMAKE_VERBOSE_MAKEFILE 0) # Use 1 for debugging, 0 for release

#
# Project Output Paths
#
set (MAINFOLDER ${PROJECT_SOURCE_DIR})
set (EXECUTABLE_OUTPUT_PATH "${MAINFOLDER}/bin")
set (LIBRARY_OUTPUT_PATH "${MAINFOLDER}/lib")
set (CATCH_DIR "${MAINFOLDER}/thirdparity/catch")

#
# Compiler flags for code coverage
#
set(GCC_COVERAGE_COMPILE_FLAGS "-coverage")
set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage -fopenmp")
set(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage -fopenmp")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

#
# Project Search Paths
#
list (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}")
include_directories("${MAINFOLDER}/include")


#
# Add Build Targets
#
add_subdirectory("${MAINFOLDER}/src")

enable_testing()
add_subdirectory("${MAINFOLDER}/test")


#
# Add Install Targets
#
if (EXISTS "${MAINFOLDER}/include/${PROJECT_NAME}" AND IS_DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}")
    install(DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}" DESTINATION "include")
endif (EXISTS "${MAINFOLDER}/include/${PROJECT_NAME}" AND IS_DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}")

#================================================================================
# Author: Alexander Miro (alexander.miro@gmail.com)
# Date: Feb-2021
#================================================================================
cmake_minimum_required(VERSION 3.16.3)
project(fsm VERSION 0.1.0 LANGUAGES CXX)

#================================================================================
# Preamble
#================================================================================
set(CMAKE_VERBOSE_MAKEFILE      true)
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        NO)
set(CMAKE_BUILD_TYPE            Debug)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    
#================================================================================
# Modules
#================================================================================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(FetchDependencies)
fetch_dependencies(DIR ${CMAKE_SOURCE_DIR}/deps GOOGLETEST release-1.10.0)

#================================================================================
# Code
#================================================================================
add_subdirectory(src)


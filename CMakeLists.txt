set(ProjectName UnqliteWrapper)
project(${ProjectName})

cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type(Debug, Release)" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB HEADER_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB SOURCE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

set(BOOST_ROOT_HINT "${BOOST_ROOT_HINT}" CACHE PATH "Boost path hint" FORCE)
set(BOOST_ROOT "${BOOST_ROOT_HINT}")
find_package(Boost 1.45.0 REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})

add_library(${ProjectName} STATIC ${HEADER_LIST} ${SOURCE_LIST})

option(BUILD_EXAMPLE "building example executable" ON)
if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

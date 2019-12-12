set(ProjectName UnqliteWrapper)
project(${ProjectName})

cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type(Debug, Release)" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

option(UNQLITE_ENABLE_THREADS "enabling thread support for unqlite" ON)

if (UNQLITE_ENABLE_THREADS)
    add_definitions(-DUNQLITE_ENABLE_THREADS)
    set(EXTERNAL_LINK_LIBRARIES ${EXTERNAL_LINK_LIBRARIES} pthread)
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/unqlite)

file(GLOB HEADER_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB SOURCE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/unqlite/unqlite.c)

add_library(${ProjectName} STATIC ${HEADER_LIST} ${SOURCE_LIST})
target_link_libraries(${ProjectName} ${EXTERNAL_LINK_LIBRARIES})

option(BUILD_EXAMPLE "building example executable" ON)
if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

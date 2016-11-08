set(ProjectName example)
project(${ProjectName})

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(${ProjectName} main.cpp)
target_link_libraries(${ProjectName} UnqliteWrapper)

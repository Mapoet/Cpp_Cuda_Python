#CMakeLists.txt
cmake_minimum_required(VERSION 3.7)
project(untitled)

# Find necessary packages
find_package(PythonLibs 3.8 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIR})
find_package(Boost COMPONENTS python REQUIRED)
find_package(Boost COMPONENTS numpy REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES cpp_example.cpp)
set(PYTHON_INCLUDE_DIR "/usr/include/python3.8")

add_library(cpp_example SHARED ${SOURCE_FILES})
target_link_libraries(cpp_example ${Boost_LIBRARIES})
set_target_properties(cpp_example PROPERTIES PREFIX "")
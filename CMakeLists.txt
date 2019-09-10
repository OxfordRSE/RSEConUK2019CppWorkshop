cmake_minimum_required(VERSION 3.14)
project(RSEConUK2019CppWorkshop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(checkpoint_99 checkpoint_99/main.cpp)

# Currently need to link in TBB for parallel STL
find_library(TBB NAMES libtbb.so)
target_link_libraries(checkpoint_99 ${TBB})

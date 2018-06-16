cmake_minimum_required(VERSION 3.5)
project(OSLab CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory(Lab2)
add_subdirectory(Lab3)
add_subdirectory(Lab4)

cmake_minimum_required(VERSION 3.17)
project(Lab9 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -lefence")
add_executable(Lab9 pro2.c)
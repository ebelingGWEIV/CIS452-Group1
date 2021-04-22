cmake_minimum_required(VERSION 3.17)
project(Lab14 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -lefence")
add_executable(Lab14 main.c)
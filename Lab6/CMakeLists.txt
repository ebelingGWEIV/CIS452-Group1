cmake_minimum_required(VERSION 3.17)
project(Lab6 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
add_executable(Lab6 sample1.c sample1.c)
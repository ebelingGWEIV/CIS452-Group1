cmake_minimum_required(VERSION 3.17)
project(Lab7 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lrt")
add_executable(Lab7 getSysInfo.c)
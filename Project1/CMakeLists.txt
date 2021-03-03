cmake_minimum_required(VERSION 3.17)
project(Project1 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(Project1 main.c)
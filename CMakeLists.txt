cmake_minimum_required(VERSION 3.17)
project(CIS452_Group1 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

set(CMAKE_C_STANDARD 11)

add_executable(CIS452_Group1 Lab4/proAssign.c)

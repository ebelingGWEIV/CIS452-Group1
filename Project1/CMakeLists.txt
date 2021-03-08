cmake_minimum_required(VERSION 3.17)
project(Project1 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
#
#set( #Insert header files here
#        TARGET_H
#        ProcessNode.h
#        SharedMem.h
#)
#set( #insert source files here
#        TARGET_SRC
#)


add_executable(Project1 Token.h SharedMem.h SigHandler.h Semaphore.h parent.c )
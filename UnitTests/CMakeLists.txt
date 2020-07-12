cmake_minimum_required(VERSION 3.10.2)

set(This 3D_Printer_Embedded)

project(${This} VERSION 0.1)

enable_testing()

add_subdirectory(googletest)

include_directories(../Inc)

set(Headers 
    ../Inc/FIFO_void.h
)

set(Sources 
    ../Src/FIFO_void.c
)

#test will be a program
#tested code will be a library for this program

add_library(${This} STATIC ${Headers} ${Sources})


#tests
add_subdirectory(Tests/fifo)

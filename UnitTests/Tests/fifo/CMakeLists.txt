cmake_minimum_required(VERSION 3.10.2)

set(This FIFO_void_UT)

#***** variables *****#
set(APP_DIR ../../..)
set(SOURCE_DIR ${APP_DIR}/Src)


#***** library *****#
include_directories(./stubs)
include_directories(${APP_DIR}/Inc)

add_library(FIFO_void_UT_lib 
    ${SOURCE_DIR}/FIFO_void.c
)


#***** test *****#
add_executable(${This} 
    FIFO_void_UT.cpp
)

target_link_libraries(${This} PUBLIC
    gtest_main
    FIFO_void_UT_lib
)

add_test(
    NAME ${This}
    COMMAND ${This}
)

cmake_minimum_required(VERSION 3.10.2)

set(This a4988_stepstick_UT)

#***** variables *****#
set(APP_DIR ../../..)
set(SOURCE_DIR ${APP_DIR}/Src)


#***** library *****#
include_directories(./stubs)
include_directories(${APP_DIR}/Inc)

add_library(a4988_stepstick_UT_lib 
    ${SOURCE_DIR}/a4988_stepstick.c
)


#***** test *****#
add_executable(${This} 
    a4988_stepstick_UT.cpp
    ${SOURCE_DIR}/ProjectObjects.c
)

target_link_libraries(${This} PUBLIC
    gtest_main
    gmock_main
    a4988_stepstick_UT_lib
)

add_test(
    NAME ${This}
    COMMAND ${This}
)

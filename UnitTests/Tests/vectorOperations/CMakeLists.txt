cmake_minimum_required(VERSION 3.10.2)

set(This vectorOperations_UT)

#***** variables *****#
set(APP_DIR ../../..)
set(SOURCE_DIR ${APP_DIR}/Src)


#***** library *****#
include_directories(${APP_DIR}/Inc)

add_library(vectorOperations_UT_lib 
    ${SOURCE_DIR}/vectorOperations.c
)


#***** test *****#
add_executable(${This} 
    vectorOperations_UT.cpp
)

target_link_libraries(${This} PUBLIC
    gtest_main
    vectorOperations_UT_lib
)

add_test(
    NAME ${This}
    COMMAND ${This}
)

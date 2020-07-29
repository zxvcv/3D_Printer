cmake_minimum_required(VERSION 3.10.2)

set(This manager_UT)

#***** variables *****#
set(APP_DIR ../../..)
set(SOURCE_DIR ${APP_DIR}/Src)


#***** library *****#
include_directories(./stubs)
include_directories(${APP_DIR}/Inc)
include_directories(${APP_DIR}/Drivers/FATFS)

add_library(manager_UT_lib 
    ${SOURCE_DIR}/manager.c
    ${SOURCE_DIR}/FIFO_void.c

)


#***** test *****#
add_executable(${This} 
    manager_UT.cpp
    #${SOURCE_DIR}/ProjectObjects.c
)

target_link_libraries(${This} PUBLIC
    gtest_main
    gmock_main
    manager_UT_lib
)

add_test(
    NAME ${This}
    COMMAND ${This}
)

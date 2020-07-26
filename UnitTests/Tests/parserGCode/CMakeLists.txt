cmake_minimum_required(VERSION 3.10.2)

set(This parserGCode_UT)

#***** variables *****#
set(APP_DIR ../../..)
set(SOURCE_DIR ${APP_DIR}/Src)


#***** library *****#
include_directories(./stubs)
include_directories(${APP_DIR}/Inc)
include_directories(${APP_DIR}/Drivers/FATFS)

add_library(parserGCode_UT_lib 
    ${SOURCE_DIR}/parserCommand.c
    ${SOURCE_DIR}/parserGCode.c
    ${SOURCE_DIR}/ProjectTypes.c
    ${SOURCE_DIR}/FIFO_void.c
    ${SOURCE_DIR}/EEPROM_24AA01.c
    ${SOURCE_DIR}/vectorOperations.c
)


#***** test *****#
add_executable(${This} 
    parserGCode_UT.cpp
    ${SOURCE_DIR}/ProjectObjects.c
)

target_link_libraries(${This} PUBLIC
    gtest_main
    gmock_main
    parserGCode_UT_lib
)

add_test(
    NAME ${This}
    COMMAND ${This}
)

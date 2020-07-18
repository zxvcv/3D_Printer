cmake_minimum_required(VERSION 3.10.2)

set(This EEPROM_24AA01_UT)

#***** variables *****#
set(APP_DIR ../../..)
set(SOURCE_DIR ${APP_DIR}/Src)


#***** library *****#
include_directories(./stubs)
include_directories(${APP_DIR}/Inc)

add_library(EEPROM_24AA01_UT_lib 
    ${SOURCE_DIR}/EEPROM_24AA01.c
    ${SOURCE_DIR}/ProjectTypes.c
)


#***** test *****#
add_executable(${This} 
    EEPROM_24AA01_UT.cpp
    ${SOURCE_DIR}/ProjectObjects.c
)

target_link_libraries(${This} PUBLIC
    gtest_main
    gmock_main
    EEPROM_24AA01_UT_lib
)

add_test(
    NAME ${This}
    COMMAND ${This}
)

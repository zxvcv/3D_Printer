# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET GCode_Parser_UT)
set(TEST_COMPONENT_LIB ${LOCAL_TARGET}lib)
set(COMPONENT_DIR ..)
set(ALL_COMPONENTS_DIR ../..)
set(MAIN_APP_DIR ../../..)
set(SOURCE_DIR ${COMPONENT_DIR}/source)
set(HEADER_DIR ${COMPONENT_DIR}/header)



#####################################   TESTED COMPONENT LIBRARY   ################################
include_directories(
    stubs
    ${HEADER_DIR}
    ${SOURCE_DIR}
)

add_library(${TEST_COMPONENT_LIB}
    ${SOURCE_DIR}/GCode_Parser.c

    ${SOURCE_DIR}/G1.c
    ${SOURCE_DIR}/G28.c
    ${SOURCE_DIR}/G90.c
    ${SOURCE_DIR}/G91.c
    ${SOURCE_DIR}/G92.c
    ${SOURCE_DIR}/M104.c
    ${SOURCE_DIR}/M106.c
    ${SOURCE_DIR}/M109.c
    ${SOURCE_DIR}/M140.c
    ${SOURCE_DIR}/M190.c
)

target_link_libraries(${TEST_COMPONENT_LIB} PRIVATE
    Mock_HAL_Drivers
    Mock_Project_Config
    Mock_Error_Codes
    Mock_A4988_stepstick
    Mock_Vector_Math
)


#####################################   TEST   ####################################################
add_executable(${LOCAL_TARGET}
    GCode_Parser_UT.cpp
)

target_link_libraries(${LOCAL_TARGET} PRIVATE
    ${TEST_COMPONENT_LIB}
    gtest_main
    gmock_main

    Mock_HAL_Drivers
    Mock_Project_Config
    Mock_Error_Codes
    Mock_A4988_stepstick
    Mock_Vector_Math
)

add_test(NAME ${LOCAL_TARGET}
    COMMAND ${LOCAL_TARGET}
)

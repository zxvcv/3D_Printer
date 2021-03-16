# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET Command_Parser_UT)
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
)

add_library(${TEST_COMPONENT_LIB}
    ${SOURCE_DIR}/Command_Parser.c
)

target_link_libraries(${TEST_COMPONENT_LIB} PRIVATE
    Mock_Project_Config
    Mock_Error_Codes
)



#####################################   TEST   ####################################################
add_executable(${LOCAL_TARGET}
    Command_Parser_UT.cpp
)

target_link_libraries(${LOCAL_TARGET} PRIVATE
    ${TEST_COMPONENT_LIB}
    gtest_main
    gmock_main

    Mock_Project_Config
    Mock_Error_Codes
)

add_test(NAME ${LOCAL_TARGET}
    COMMAND ${LOCAL_TARGET}
)

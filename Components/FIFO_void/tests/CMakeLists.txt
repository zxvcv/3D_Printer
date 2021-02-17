# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET FIFO_void_UT)
set(TEST_COMPONENT_LIB ${LOCAL_TARGET}lib)
set(COMPONENTS_DIR ..)
set(SOURCE_DIR ${COMPONENTS_DIR}/source)
set(HEADER_DIR ${COMPONENTS_DIR}/header)



#####################################   TESTED COMPONENT LIBRARY   ################################
include_directories(
    stubs
    mocs
    ${HEADER_DIR}
)

add_library(${TEST_COMPONENT_LIB}
    ${SOURCE_DIR}/FIFO_void_C.c
    ${SOURCE_DIR}/FIFO_void_NC.c
    ${SOURCE_DIR}/FIFO_void_Shared.c
)



#####################################   TEST   ####################################################
add_executable(${LOCAL_TARGET} 
    FIFO_void_UT.cpp
)

target_link_libraries(${LOCAL_TARGET} PUBLIC
    gtest_main
    ${TEST_COMPONENT_LIB}
)

add_test(NAME ${LOCAL_TARGET}
    COMMAND ${LOCAL_TARGET}
)

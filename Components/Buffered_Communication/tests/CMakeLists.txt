# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET Buffered_Communication_UT)
set(TEST_COMPONENT_LIB ${LOCAL_TARGET}lib)
set(COMPONENT_DIR ..)
set(ALL_COMPONENTS_DIR ../..)
set(MAIN_APP_DIR ../../..)
set(SOURCE_DIR ${COMPONENT_DIR}/source)
set(HEADER_DIR ${COMPONENT_DIR}/header)



#####################################   TESTED COMPONENT LIBRARY   ################################
list(APPEND EXTERNAL_LIBS
    FIFO_void
    #Error_Codes
    #Project_Config
)

include_directories(
    stubs
    ${HEADER_DIR}
    ${MAIN_APP_DIR}/Drivers/STM32F3xx_HAL_Driver/tests/mocks
    ${MAIN_APP_DIR}/Drivers/STM32F3xx_HAL_Driver/tests/stubs

    #TEMPORARY:
    #${ALL_COMPONENTS_DIR}/Error_Codes/tests/mocks
    ${ALL_COMPONENTS_DIR}/Error_Codes/header
    #${ALL_COMPONENTS_DIR}/Project_Config/tests/mocks
    ${ALL_COMPONENTS_DIR}/Project_Config/header
)
foreach(LIBRARY IN LISTS EXTERNAL_LIBS)
    include_directories(
        ${ALL_COMPONENTS_DIR}/${LIBRARY}/tests/mocks
        ${ALL_COMPONENTS_DIR}/${LIBRARY}/header
    )
endforeach()

add_library(${TEST_COMPONENT_LIB}
    ${SOURCE_DIR}/Buffered_Communication.c
)

target_compile_definitions(${TEST_COMPONENT_LIB} PRIVATE -DTESTS)



#####################################   TEST   ####################################################
add_executable(${LOCAL_TARGET}
    Buffered_Communication_UT.cpp
    ${MAIN_APP_DIR}/Drivers/STM32F3xx_HAL_Driver/tests/mocks/Mock_HAL_Drivers.cpp
    ${ALL_COMPONENTS_DIR}/FIFO_void/tests/mocks/Mock_FIFO_void.cpp
)

target_link_libraries(${LOCAL_TARGET} PUBLIC
    gtest_main
    gmock_main
    ${TEST_COMPONENT_LIB}
)

add_test(NAME ${LOCAL_TARGET}
    COMMAND ${LOCAL_TARGET}
)

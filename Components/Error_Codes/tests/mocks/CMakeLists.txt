# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET Mock_Error_Codes)
set(SOURCE_DIR .)
set(HEADER_DIR .)
set(COMPONENT_DIR ../..)
set(COMPONENTS_DIR ../../..)
set(MAIN_APP_DIR ../../../..)



#####################################   CREATE MOCK LIBRARY TARGET   ##############################
add_library(${LOCAL_TARGET}
    ${SOURCE_DIR}/Mock_Error_Codes.cpp
)

target_include_directories(${LOCAL_TARGET} PUBLIC
    ${HEADER_DIR}
    ${COMPONENT_DIR}/header
    #${MAIN_APP_DIR}/Drivers/STM32F3xx_HAL_Driver/tests/stubs
)

target_link_libraries(${LOCAL_TARGET} PRIVATE
    gmock_main
    Mock_HAL_Drivers
    Mock_Project_Config
)
# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET Mock_HAL_Drivers)
set(SOURCE_DIR .)
set(HEADER_DIR .)
set(COMPONENT_DIR ../..)
set(MAIN_APP_DIR ../../../..)



#####################################   CREATE MOCK LIBRARY TARGET   ##############################
add_library(${LOCAL_TARGET}
    ${SOURCE_DIR}/Mock_HAL_Drivers.cpp
)

target_include_directories(${LOCAL_TARGET} PUBLIC
    ${HEADER_DIR}
    ${COMPONENT_DIR}/tests/stubs
)

target_link_libraries(${LOCAL_TARGET} PRIVATE
    gmock_main
)
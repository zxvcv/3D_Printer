# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET EEPROM_24AA01)
set(COMPONENTS_DIR ..)
set(SOURCE_DIR source)
set(HEADER_DIR header)



#####################################   COMPILATION OPTIONS   #####################################
add_library(${LOCAL_TARGET}
    ${SOURCE_DIR}/EEPROM_24AA01.c
)

target_include_directories(${LOCAL_TARGET} PUBLIC
    ${HEADER_DIR}
)

target_link_libraries(${LOCAL_TARGET}
    Error_Codes
    HAL_Drivers
)



#####################################   COMPILATION OPTIONS   #####################################
include(../../cmake/compile_definitions.cmake)
include(../../cmake/compile_options.cmake)

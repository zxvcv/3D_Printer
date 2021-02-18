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

    ${HEADER_DIR}/EEPROM_24AA01.h
)

target_include_directories(${LOCAL_TARGET} PRIVATE
    ${HEADER_DIR}
)

add_dependencies(${LOCAL_TARGET}
    Error_Codes
    Drivers
)



#####################################   COMPILATION OPTIONS   #####################################
include(../../cmake/compile_options.cmake)

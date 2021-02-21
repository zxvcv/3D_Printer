# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET GCode_Parser)
set(MAIN_APP_DIR ../..)
set(SOURCE_DIR source)
set(HEADER_DIR header)



#####################################   INCLUDES   ################################################
include(${MAIN_APP_DIR}/cmake/f_compile_definitions.cmake)
include(${MAIN_APP_DIR}/cmake/f_compile_options.cmake)



#####################################   CREATE TARGET   ###########################################
add_library(${LOCAL_TARGET}
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

target_include_directories(${LOCAL_TARGET} PUBLIC
    ${HEADER_DIR}
    ${MAIN_APP_DIR}/Inc
)

target_link_libraries(${LOCAL_TARGET}
    Error_Codes
    A4988_stepstick
    EEPROM_24AA01
    Vector_Math
    SD_Card
    FATFS_Driver #[DEBUG]
    Buffered_Communication
)



#####################################   COMPILATION SETTINGS   ####################################
set_target_compile_definitions(${LOCAL_TARGET})
set_target_compile_options(${LOCAL_TARGET})

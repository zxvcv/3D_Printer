# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET Command_Parser)
set(MAIN_APP_DIR ../..)
set(SOURCE_DIR source)
set(HEADER_DIR header)



#####################################   INCLUDES   ################################################
include(${MAIN_APP_DIR}/cmake/f_compile_definitions.cmake)
include(${MAIN_APP_DIR}/cmake/f_compile_options.cmake)



#####################################   CREATE TARGET   ###########################################
add_library(${LOCAL_TARGET}
    ${SOURCE_DIR}/Command_Parser.c

    ${SOURCE_DIR}/MotorDataRequest.c
    ${SOURCE_DIR}/MotorDistanceMove.c
    ${SOURCE_DIR}/MotorPositionEnd.c
    ${SOURCE_DIR}/MotorPositionMove.c
    ${SOURCE_DIR}/MotorPositionValueSet.c
    ${SOURCE_DIR}/MotorPositionZero.c
    ${SOURCE_DIR}/MotorSpeedMax.c
    ${SOURCE_DIR}/MotorSpeedSet.c
    ${SOURCE_DIR}/MotorStepSizeRequest.c
    ${SOURCE_DIR}/MotorStepSizeSet.c
    ${SOURCE_DIR}/SDCardProgramRun.c
)

target_include_directories(${LOCAL_TARGET} PUBLIC
    ${HEADER_DIR}
)

target_link_libraries(${LOCAL_TARGET}
    Error_Codes
    A4988_stepstick
    EEPROM_24AA01
    Buffered_Communication
    SD_Card
    FATFS_Driver #[DEBUG]
)



#####################################   COMPILATION SETTINGS   ####################################
set_target_compile_definitions(${LOCAL_TARGET})
set_target_compile_options(${LOCAL_TARGET})

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

    ${SOURCE_DIR}/reusable.c
    ${SOURCE_DIR}/U00.c
    ${SOURCE_DIR}/U01.c
    ${SOURCE_DIR}/U02.c
    ${SOURCE_DIR}/U10.c
    ${SOURCE_DIR}/U11.c
    ${SOURCE_DIR}/U20.c
    ${SOURCE_DIR}/U21.c
    ${SOURCE_DIR}/U22.c
    ${SOURCE_DIR}/U23.c
    ${SOURCE_DIR}/U24.c
    ${SOURCE_DIR}/U40.c
)

target_include_directories(${LOCAL_TARGET} PUBLIC
    ${HEADER_DIR}
)

target_link_libraries(${LOCAL_TARGET}
    Error_Codes
    Project_Config
    Buffered_Communication
    GCode_Parser
    Manager_EEPROM
    Manager_SDcard
)



#####################################   COMPILATION SETTINGS   ####################################
set_target_compile_definitions(${LOCAL_TARGET})
set_target_compile_options(${LOCAL_TARGET})

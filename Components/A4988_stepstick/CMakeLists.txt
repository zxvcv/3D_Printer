# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET A4988_stepstick)
set(MAIN_APP_DIR ../..)
set(SOURCE_DIR source)
set(HEADER_DIR header)



#####################################   INCLUDES   ################################################
include(${MAIN_APP_DIR}/cmake/f_compile_definitions.cmake)
include(${MAIN_APP_DIR}/cmake/f_compile_options.cmake)



#####################################   CREATE TARGET   ###########################################
add_library(${LOCAL_TARGET}
    ${SOURCE_DIR}/A4988_stepstick.c
)

target_include_directories(${LOCAL_TARGET} PUBLIC
    ${HEADER_DIR}
)

target_link_libraries(${LOCAL_TARGET}
    HAL_Drivers
    Error_Codes
    IOpin
)



#####################################   COMPILATION SETTINGS   ####################################
set_target_compile_definitions(${LOCAL_TARGET})
set_target_compile_options(${LOCAL_TARGET})

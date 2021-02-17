# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)
project(3D_Printer_UT VERSION 0.1.0)
enable_testing()

#####################################   LOCAL VARIABLES   #########################################
set(COMPONENTS_DIR ../Components)
set(TESTS_DIR Tests)



#####################################   SUBDIRECTORIES   ##########################################
add_subdirectory(googletest ../bld_googletest)



#####################################   TESTS   ###################################################
list(APPEND TESTS
    FIFO_void
    Error_Codes
    #---add new tests here---
)



#####################################   TEST ADD   ################################################
foreach(TEST IN LISTS TESTS)
    add_subdirectory(${COMPONENTS_DIR}/${TEST}/tests ${TESTS_DIR}/${TEST})
endforeach()

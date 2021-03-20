# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)
project(3D_Printer_UT VERSION 0.1.0)
enable_testing()

#####################################   LOCAL VARIABLES   #########################################
set(COMPONENTS_DIR ../Components)
set(TESTS_DIR Tests)
set(MOCKS_DIR Mocks)



#####################################   SUBDIRECTORIES   ##########################################
add_subdirectory(googletest ../bld_googletest)



#####################################   TESTS   ###################################################
list(APPEND TESTS
    A4988_stepstick
    Buffered_Communication
    Command_Parser
    EEPROM_24AA01
    Error_Codes
    FIFO_void
    GCode_Parser
    IOpin
    Project_Config
    Vector_Math
    #---add new tests here---
)

list(APPEND MOCKS
    A4988_stepstick
    Buffered_Communication
    Command_Parser
    EEPROM_24AA01
    Error_Codes
    FIFO_void
    GCode_Parser
    IOpin
    Manager_Communication
    Manager_EEPROM
    Manager_SDcard
    Project_Config
    Vector_Math
    #---add new mocks here---
)



#####################################   TEST ADD   ################################################
foreach(TEST IN LISTS TESTS)
    add_subdirectory(${COMPONENTS_DIR}/${TEST}/tests ${TESTS_DIR}/${TEST})
endforeach()
foreach(MOCK IN LISTS MOCKS)
    add_subdirectory(${COMPONENTS_DIR}/${MOCK}/tests/mocks ${MOCKS_DIR}/Mock_${MOCK})
endforeach()
add_subdirectory(../Drivers/STM32F3xx_HAL_Driver/tests/mocks ${MOCKS_DIR}/Mock_HAL_Drivers)
add_subdirectory(../Drivers/FATFS/tests/mocks ${MOCKS_DIR}/Mock_SD)
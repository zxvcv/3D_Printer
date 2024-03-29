# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)
project(3D_Printer VERSION 0.1.0)

#####################################   INCLUDES   ################################################
include(cmake/f_compile_definitions.cmake)
include(cmake/f_compile_options.cmake)



#####################################   LANGUAGE AND STANDARD   ###################################
enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)



#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET ${PROJECT_NAME})
set(COMPONENTS_DIR Components)
set(LDSCRIPT STM32F303RETx_FLASH.ld)



#####################################   LIBRARIES   ###############################################
list(APPEND LIBRARIES
    A4988_stepstick
    Boundary_Detector
    Buffered_Communication
    Command_Parser
    EEPROM_24AA01
    Error_Codes
    FIFO_void
    GCode_Parser
    Geometry_Math
    IOpin
    Manager_BoundariesDetector
    Manager_Communication
    Manager_EEPROM
    Manager_SDcard
    Project_Config
    Vector_Math
    #---add new libs here---
)



#####################################   SUBDIRECTORIES   ##########################################
add_subdirectory(Drivers) #HAL_Drivers, FATFS_Driver
foreach(LIBRARY IN LISTS LIBRARIES)
    add_subdirectory(${COMPONENTS_DIR}/${LIBRARY})
endforeach()



#####################################   EXECUTABLE   ##############################################
add_executable(${LOCAL_TARGET}
    Src/main.c
    Src/Interrupts.c
    Src/Init_Manager.c
    Src/Manager.c
    Src/Project_Objects.c

    Src/stm32f3xx_it.c
    Src/stm32f3xx_hal_msp.c
    Src/system_stm32f3xx.c

    Inc/main.h
    Inc/stm32f3xx_hal_conf.h
    Inc/stm32f3xx_it.h

    startup_stm32f303xe.s
)

target_include_directories(${LOCAL_TARGET} PUBLIC
    Inc
)

target_link_libraries(${LOCAL_TARGET}
    HAL_Drivers
    FATFS_Driver
    ${LIBRARIES}
)



#####################################   COMPILATION SETTINGS   ####################################
set_target_compile_definitions(${LOCAL_TARGET})
set_target_compile_options(${LOCAL_TARGET})

target_link_options(${LOCAL_TARGET} PRIVATE
    -T${CMAKE_SOURCE_DIR}/${LDSCRIPT}
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -specs=nano.specs
    -specs=nosys.specs
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -u _printf_float
)



#####################################   PRINT EXECUTABLE SIZE   ###################################
add_custom_command(TARGET ${LOCAL_TARGET}
    POST_BUILD
    COMMAND arm-none-eabi-size ${LOCAL_TARGET}
)



#####################################   CREATE HEX FILE   ###################################
add_custom_command(TARGET ${LOCAL_TARGET} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${LOCAL_TARGET} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${LOCAL_TARGET} ${PROJECT_NAME}.bin
)

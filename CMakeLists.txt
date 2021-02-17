# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)
project(3D_Printer VERSION 0.1.0)

#####################################   LANGUAGE AND STANDARD   ###################################
enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)



#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET ${PROJECT_NAME})
set(COMPONENTS_DIR Components)



#####################################   GENERATED FILES LIST   ####################################
set(LDSCRIPT STM32F303RETx_FLASH.ld)
set(STM32CUBEMX_GENERATED_FILES
    Src/main.c
    Src/stm32f3xx_it.c
    Src/stm32f3xx_hal_msp.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi_ex.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c
    Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c
    Src/system_stm32f3xx.c

    Inc/main.h
    Inc/stm32f3xx_hal_conf.h
    Inc/stm32f3xx_it.h

    startup_stm32f303xe.s
)



#####################################   LIBRARIES   ###############################################
list(APPEND LIBRARIES
    FIFO_void
    Error_Codes
    #---add new libs here---
)



#####################################   SUBDIRECTORIES   ##########################################
foreach(LIBRARY IN LISTS LIBRARIES)
    add_subdirectory(${COMPONENTS_DIR}/${LIBRARY})
endforeach()



#####################################   EXECUTABLE   ##############################################
add_executable(${LOCAL_TARGET}
    ${STM32CUBEMX_GENERATED_FILES}
)

target_include_directories(${LOCAL_TARGET} PRIVATE
    Inc
    Drivers/STM32F3xx_HAL_Driver/Inc
    Drivers/STM32F3xx_HAL_Driver/Inc/Legacy
    Drivers/CMSIS/Device/ST/STM32F3xx/Include
    Drivers/CMSIS/Include
)

target_link_libraries(${LOCAL_TARGET} PUBLIC
    ${LIBRARIES}
)

target_compile_definitions(${LOCAL_TARGET} PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32F303xE
)



#####################################   COMPILATION SETTINGS   #####################################
include(cmake/compile_options.cmake)

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

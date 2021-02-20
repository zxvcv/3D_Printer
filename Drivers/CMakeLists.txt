# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET HAL_Drivers)
set(MAIN_APP_DIR ..)
set(HAL_DRIVER_DIR STM32F3xx_HAL_Driver)
set(CMSIS_DRIVER_DIR CMSIS)
set(SOURCE_HAL_DIR ${HAL_DRIVER_DIR}/Src)
set(HEADER_HAL_DIR ${HAL_DRIVER_DIR}/Inc)
set(HEADER_CMSIS_DIR ${CMSIS_DRIVER_DIR}/Include)



#####################################   INCLUDES   ################################################
include(${MAIN_APP_DIR}/cmake/f_compile_definitions.cmake)
include(${MAIN_APP_DIR}/cmake/f_compile_options.cmake)



#####################################   CREATE TARGET   ###########################################
add_library(${LOCAL_TARGET}
    ${SOURCE_HAL_DIR}/stm32f3xx_hal.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_i2c.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_i2c_ex.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_rcc.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_rcc_ex.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_gpio.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_dma.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_cortex.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_pwr.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_pwr_ex.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_flash.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_flash_ex.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_exti.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_spi.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_spi_ex.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_tim.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_tim_ex.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_uart.c
    ${SOURCE_HAL_DIR}/stm32f3xx_hal_uart_ex.c
)

target_include_directories(${LOCAL_TARGET} PUBLIC
    ${HEADER_HAL_DIR}
    ${HEADER_HAL_DIR}/Legacy
    ${CMSIS_DRIVER_DIR}/Device/ST/STM32F3xx/Include
    ${HEADER_CMSIS_DIR}
    ../Inc
)



#####################################   COMPILATION SETTINGS   ####################################
set_target_compile_definitions(${LOCAL_TARGET})
set_target_compile_options(${LOCAL_TARGET})

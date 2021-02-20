# Copyright (c) 2020-2021 Paweł Piskorz
# Licensed under the Eclipse Public License 2.0
# See attached LICENSE file

cmake_minimum_required(VERSION 3.16.3)

#####################################   LOCAL VARIABLES   #########################################
set(LOCAL_TARGET_HAL HAL_Drivers)
set(LOCAL_TARGET_FATFS FATFS_Driver)
set(MAIN_APP_DIR ..)
set(HAL_DRIVER_DIR STM32F3xx_HAL_Driver)
set(CMSIS_DRIVER_DIR CMSIS)
set(HAL_SOURCE_DIR ${HAL_DRIVER_DIR}/Src)
set(HAL_HEADER_DIR ${HAL_DRIVER_DIR}/Inc)
set(CMSIS_HEADER_DIR ${CMSIS_DRIVER_DIR}/Include)
set(FATFS_DRIVER_DIR FATFS)
set(FATFS_SOURCE_DIR ${FATFS_DRIVER_DIR}/Src)
set(FATFS_HEADER_DIR ${FATFS_DRIVER_DIR}/Inc)
set(FATFS_LL_DRIVER_DIR ${FATFS_DRIVER_DIR}/STM32_lowlayer)
set(FATFS_LL_SOURCE_DIR ${FATFS_LL_DRIVER_DIR}/Src)
set(FATFS_LL_HEADER_DIR ${FATFS_LL_DRIVER_DIR}/Inc)



#####################################   INCLUDES   ################################################
include(${MAIN_APP_DIR}/cmake/f_compile_definitions.cmake)
include(${MAIN_APP_DIR}/cmake/f_compile_options.cmake)



#####################################   CREATE LOCAL TARGET HAL_DRIVERS   #########################
add_library(${LOCAL_TARGET_HAL}
    ${HAL_SOURCE_DIR}/stm32f3xx_hal.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_i2c.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_i2c_ex.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_rcc.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_rcc_ex.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_gpio.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_dma.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_cortex.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_pwr.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_pwr_ex.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_flash.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_flash_ex.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_exti.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_spi.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_spi_ex.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_tim.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_tim_ex.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_uart.c
    ${HAL_SOURCE_DIR}/stm32f3xx_hal_uart_ex.c
)

target_include_directories(${LOCAL_TARGET_HAL} PUBLIC
    ${HAL_HEADER_DIR}
    ${HAL_HEADER_DIR}/Legacy
    ${CMSIS_DRIVER_DIR}/Device/ST/STM32F3xx/Include
    ${CMSIS_HEADER_DIR}
    ../Inc
)



#####################################   CREATE LOCAL TARGET FATFS_DRIVERS   #######################
add_library(${LOCAL_TARGET_FATFS}
    ${FATFS_SOURCE_DIR}/ff.c
    ${FATFS_SOURCE_DIR}/ffsystem.c
    ${FATFS_SOURCE_DIR}/ffunicode.c

    ${FATFS_LL_SOURCE_DIR}/diskio.c
    ${FATFS_LL_SOURCE_DIR}/SD.c
)

target_include_directories(${LOCAL_TARGET_FATFS} PUBLIC
    ${FATFS_HEADER_DIR}
    ${FATFS_LL_HEADER_DIR}
    ${MAIN_APP_DIR}/Inc
)

target_link_libraries(${LOCAL_TARGET_FATFS}
    HAL_Drivers
    FIFO_void
    IOpin
)



#####################################   COMPILATION SETTINGS   ####################################
set_target_compile_definitions(${LOCAL_TARGET_HAL})
set_target_compile_options(${LOCAL_TARGET_HAL})

set_target_compile_definitions(${LOCAL_TARGET_FATFS})
set_target_compile_options(${LOCAL_TARGET_FATFS})

#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "stm32f3xx_hal.h"
}

class Mock_HAL_Drivers {
public:
    Mock_HAL_Drivers() { mock = this; }
    MOCK_METHOD3(HAL_UART_Receive_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData,
        uint16_t Size));
    MOCK_METHOD3(HAL_UART_Transmit_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData,
        uint16_t Size));
    MOCK_METHOD7(HAL_I2C_Mem_Write, HAL_StatusTypeDef(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
        uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout));
    MOCK_METHOD7(HAL_I2C_Mem_Read, HAL_StatusTypeDef(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
        uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout));
    MOCK_METHOD3(HAL_GPIO_WritePin, void(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState));

    static Mock_HAL_Drivers* mock;
};

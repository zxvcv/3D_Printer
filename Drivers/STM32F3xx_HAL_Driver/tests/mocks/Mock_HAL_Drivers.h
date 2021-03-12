#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "stm32f3xx_hal.h"
}

class Mock_HAL_Drivers {
public:
    Mock_HAL_Drivers() { mock = this; }
    MOCK_METHOD3(HAL_UART_Receive_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size));
    MOCK_METHOD3(HAL_UART_Transmit_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size));


    static Mock_HAL_Drivers* mock;
};

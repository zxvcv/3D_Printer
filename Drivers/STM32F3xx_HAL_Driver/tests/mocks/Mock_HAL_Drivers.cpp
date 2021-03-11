#include "Mock_HAL_Drivers.h"


Mock_HAL_Drivers* Mock_HAL_Drivers::mock;

extern "C"
{
    HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        return Mock_HAL_Drivers::mock->HAL_UART_Receive_IT(huart, pData, Size);
    }
}

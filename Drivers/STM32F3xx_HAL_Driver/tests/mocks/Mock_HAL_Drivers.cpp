#include "Mock_HAL_Drivers.h"


Mock_HAL_Drivers* Mock_HAL_Drivers::mock;

extern "C"
{
    HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        return Mock_HAL_Drivers::mock->HAL_UART_Receive_IT(huart, pData, Size);
    }

    HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        return Mock_HAL_Drivers::mock->HAL_UART_Transmit_IT(huart, pData, Size);
    }

    void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
    {
        Mock_HAL_Drivers::mock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
    }
}

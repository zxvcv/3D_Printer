#ifndef __STM32F3xx_HAL_H
#define __STM32F3xx_HAL_H
/*
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif



#define HAL_MAX_DELAY      0xFFFFFFFFU

typedef enum
{
  GPIO_PIN_RESET = 0U,
  GPIO_PIN_SET
}GPIO_PinState;
*/
typedef struct
{
  
}GPIO_TypeDef;

typedef enum 
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03
} HAL_StatusTypeDef;
/*
typedef struct __I2C_HandleTypeDef
{
  
} I2C_HandleTypeDef;

typedef struct __SPI_HandleTypeDef
{
  
} SPI_HandleTypeDef;

typedef struct __UART_HandleTypeDef
{

} UART_HandleTypeDef;

typedef struct __TIM_HandleTypeDef
{

} TIM_HandleTypeDef;


I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;


HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
  uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, 
  uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
  uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, 
  uint32_t Timeout);

//#include "managerBT.h"
//void init_operations_BT(BT_Settings* settings);

*/
#endif /* __STM32F3xx_HAL_H */

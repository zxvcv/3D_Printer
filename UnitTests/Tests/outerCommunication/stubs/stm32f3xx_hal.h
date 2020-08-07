#ifndef __STM32F3xx_HAL_H
#define __STM32F3xx_HAL_H

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "ff.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/*pointers may not point to correct peripherial values, only for tests*/
#define GPIOA ((GPIO_TypeDef *) 0x40000000UL)
#define GPIOB ((GPIO_TypeDef *) 0x40000000UL)
#define GPIOC ((GPIO_TypeDef *) 0x40000000UL)
#define GPIOD ((GPIO_TypeDef *) 0x40000000UL)
#define GPIO_PIN_0 ((uint16_t)0x0004U)
#define GPIO_PIN_1 ((uint16_t)0x0004U)
#define GPIO_PIN_2 ((uint16_t)0x0004U)
#define GPIO_PIN_3 ((uint16_t)0x0004U)
#define GPIO_PIN_4 ((uint16_t)0x0004U)
#define GPIO_PIN_5 ((uint16_t)0x0004U)
#define GPIO_PIN_6 ((uint16_t)0x0004U)
#define GPIO_PIN_7 ((uint16_t)0x0004U)
#define GPIO_PIN_8 ((uint16_t)0x0004U)
#define GPIO_PIN_9 ((uint16_t)0x0004U)
#define GPIO_PIN_10 ((uint16_t)0x0004U)
#define GPIO_PIN_11 ((uint16_t)0x0004U)

#define HAL_MAX_DELAY      0xFFFFFFFFU

typedef enum
{
  GPIO_PIN_RESET = 0U,
  GPIO_PIN_SET
}GPIO_PinState;

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
/*
typedef struct {

} FATFS;

typedef struct {

} FIL;

typedef char TCHAR;
typedef unsigned char	BYTE;


*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);

HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
  uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, 
  uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
  uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, 
  uint32_t Timeout);

FRESULT f_open (FIL* fp, const TCHAR* path,	BYTE mode);
FRESULT f_mount (FATFS* fs,	const TCHAR* path, BYTE opt);

HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

#endif /* __STM32F3xx_HAL_H */

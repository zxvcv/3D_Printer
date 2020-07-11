/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define MOT1_STEP_Pin GPIO_PIN_0
#define MOT1_STEP_GPIO_Port GPIOC
#define MOT1_DIRECTION_Pin GPIO_PIN_1
#define MOT1_DIRECTION_GPIO_Port GPIOC
#define MOT1_RESET_Pin GPIO_PIN_2
#define MOT1_RESET_GPIO_Port GPIOC
#define MOT1_SLEEP_Pin GPIO_PIN_3
#define MOT1_SLEEP_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define ST7565R_CS_Pin GPIO_PIN_0
#define ST7565R_CS_GPIO_Port GPIOB
#define ST7565R_RST_Pin GPIO_PIN_1
#define ST7565R_RST_GPIO_Port GPIOB
#define ST7565R_A0_Pin GPIO_PIN_2
#define ST7565R_A0_GPIO_Port GPIOB
#define MOT3_SLEEP_Pin GPIO_PIN_6
#define MOT3_SLEEP_GPIO_Port GPIOC
#define MOT3_RESET_Pin GPIO_PIN_7
#define MOT3_RESET_GPIO_Port GPIOC
#define MOT3_DIRECTION_Pin GPIO_PIN_8
#define MOT3_DIRECTION_GPIO_Port GPIOC
#define MOT3_STEP_Pin GPIO_PIN_9
#define MOT3_STEP_GPIO_Port GPIOC
#define MOT4_SLEEP_Pin GPIO_PIN_8
#define MOT4_SLEEP_GPIO_Port GPIOA
#define MOT4_RESET_Pin GPIO_PIN_9
#define MOT4_RESET_GPIO_Port GPIOA
#define MOT4_DIRECTION_Pin GPIO_PIN_10
#define MOT4_DIRECTION_GPIO_Port GPIOA
#define MOT4_STEP_Pin GPIO_PIN_11
#define MOT4_STEP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SDSPI_CS_Pin GPIO_PIN_2
#define SDSPI_CS_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define MOT2_SLEEP_Pin GPIO_PIN_4
#define MOT2_SLEEP_GPIO_Port GPIOB
#define MOT2_RESET_Pin GPIO_PIN_5
#define MOT2_RESET_GPIO_Port GPIOB
#define MOT2_DIRECTION_Pin GPIO_PIN_6
#define MOT2_DIRECTION_GPIO_Port GPIOB
#define MOT2_STEP_Pin GPIO_PIN_7
#define MOT2_STEP_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

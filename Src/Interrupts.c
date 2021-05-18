/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Interrupts
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Interrupts.h"
#include "stm32f3xx_hal.h"
#include "Project_Objects.h"
#include "Buffered_Communication.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

extern DeviceSettings printerSettings;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim16;



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim6)
    {
        for(int i=0; i< MOTORS_NUM; ++i)
        {
            if(printerSettings.motors_are_on)
            {
                motor_update(printerSettings.motors[i]);
                /*TODO: error handling*/
            }
        }
    }

    if(htim == &htim16)
    {
        subtract_vibrations_delay_counters(printerSettings.boundaryDetection);
    }
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == printerSettings.buff_comm->huart)
    {
        send_buffered_message_IT(printerSettings.buff_comm);
    }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == printerSettings.buff_comm->huart)
    {
        receive_buffered_message_IT(printerSettings.buff_comm);
    }
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    Std_Err stdErr = STD_OK;

    stdErr = check_boundaries(printerSettings.boundaryDetection, GPIO_Pin);
    if(stdErr != STD_OK)
    {
        printerSettings.error = stdErr;
        Error_Handler();
    }
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

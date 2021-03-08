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
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#ifdef USE_INTERRUPTS
#define IRQ_ENABLE __enable_irq()
#define IRQ_DISABLE __disable_irq()
#endif /* USE_INTERRUPTS */
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

extern DeviceSettings printerSettings;



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    for(int i=0; i< MOTORS_NUM; ++i)
    {
        if(motorIsOn(printerSettings.motors[i]))
        {
            motorUpdate(printerSettings.motors[i]);
            /*TODO: error handling*/
        }
    }
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    //Std_Err stdErr;
    //HAL_StatusTypeDef halErr;
    uint8_t* data;

    #ifdef USE_INTERRUPTS
    IRQ_DISABLE;
    #endif /* USE_INTERRUPTS */

    fifo_pop_C(printerSettings.outComm->Buff_OUT);
    /*TODO: error handling*/

    #ifdef USE_INTERRUPTS
    IRQ_ENABLE;
    #endif /* USE_INTERRUPTS */

    if(fifo_getSize(printerSettings.outComm->Buff_OUT) > 1)
    {
        fifo_front(printerSettings.outComm->Buff_OUT, (void**)&data);
        /*TODO: error handling*/

        HAL_UART_Transmit_IT(printerSettings.outComm->huart,
                        data, fifo_getDataSize(printerSettings.outComm->Buff_OUT));
        /*TODO: error handling*/
    }
    else
    {
        printerSettings.outComm->transmission = false;
    }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //Std_Err stdErr;
    //HAL_StatusTypeDef halErr;

    if(huart == printerSettings.outComm->huart)
    {

        fifo_push_C(printerSettings.outComm->Buff_IN, &(printerSettings.outComm->recieved), 1);
        /*TODO: error handling*/

        if(printerSettings.outComm->recieved == '\n')
        {
            printerSettings.outComm->EOL_recieved = true;
        }

        HAL_UART_Receive_IT(printerSettings.outComm->huart, &(printerSettings.outComm->recieved), 1);
        /*TODO: error handling*/
    }
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

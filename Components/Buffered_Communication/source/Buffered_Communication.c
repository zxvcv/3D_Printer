/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Buffered_Communication
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Buffered_Communication.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err init_buffered_communication(BuffCommunication_Settings* settings, UART_HandleTypeDef* huart)
{
    Std_Err stdErr = STD_OK;
    HAL_StatusTypeDef halStatus;

    settings->huart = huart;
    settings->EOL_recieved = false;
    settings->transmission = false;

    stdErr = fifo_create(&(settings->Buff_InputCommands));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    stdErr = fifo_create(&(settings->Buff_IN));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    stdErr = fifo_create(&(settings->Buff_OUT));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    halStatus = HAL_UART_Receive_IT(settings->huart, &(settings->recieved), 1);
    if(halStatus != HAL_OK)
    {
        return translate_error_hal_to_project(halStatus);
    }

    return stdErr;
}


Std_Err send_buffered_message(BuffCommunication_Settings* settings)
{
    Std_Err stdErr = STD_OK;
    HAL_StatusTypeDef halErr;
    uint8_t* data = NULL;

    if(!settings->transmission && fifo_getSize(settings->Buff_OUT) > 0)
    {
        settings->transmission = true;
        stdErr = fifo_front(settings->Buff_OUT, (void**)&data);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }

        halErr = HAL_UART_Transmit_IT(settings->huart, data, fifo_getDataSize(settings->Buff_OUT));

        stdErr = translate_error_hal_to_project(halErr);
    }
    else if(settings->transmission)
    {
        stdErr = STD_BUSY_ERROR;
    }

    return stdErr;
}


Std_Err deinit_buffered_communication(BuffCommunication_Settings* settings)
{
    Std_Err stdErr = STD_OK;

    stdErr = fifo_delete_C(&(settings->Buff_InputCommands));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    stdErr = fifo_delete_C(&(settings->Buff_IN));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    stdErr = fifo_delete_C(&(settings->Buff_OUT));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

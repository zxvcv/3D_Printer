/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Manager
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Manager.h"
#include <stdio.h>
#include "SD_Card.h"
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
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

char buffMsg[100];
uint8_t msgSize;
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err execute_outer_command(DeviceSettings* settings)
{
    Std_Err stdErr = STD_OK;

    SystemCommand* cmd = NULL;
    uint8_t listSize;

    listSize = fifo_getSize(settings->outComm->Buff_InputCommands);

    if(listSize > 0)
    {
        stdErr = fifo_front(settings->outComm->Buff_InputCommands, (void**)&cmd);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }

        stdErr = executeSystemCommand(cmd, settings);

        if(stdErr != STD_OK)
        {
            /*TODO: log push error */
            //msgSize = sprintf(buffMsg, "<ERR> %s\n", get_std_err_string(stdErr));
            msgSize = sprintf(buffMsg, "<ERR> %s\n", "get_std_err_string(stdErr)");

            stdErr = fifo_push_C(settings->outComm->Buff_OUT, (char*)buffMsg, msgSize);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }
        }

        stdErr = fifo_pop_C(settings->outComm->Buff_InputCommands);
    }
    else
    {
        stdErr = STD_OK;
    }

    return stdErr;
}


Std_Err parse_outer_data(DeviceSettings* settings)
{
    Std_Err stdErr = STD_OK;

    SystemCommand cmd;
    uint8_t sizeTemp = 0;
    uint8_t* data;
    uint8_t temp[25];

    if(settings->outComm->EOL_recieved)
    {
        settings->outComm->EOL_recieved = false;

        do
        {
            stdErr = fifo_front(settings->outComm->Buff_IN, (void**)&data);
            temp[sizeTemp++] = *data;
            if(stdErr != STD_OK)
            {
                return stdErr;
            }
            
            stdErr = fifo_pop_C(settings->outComm->Buff_IN);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

        }while(temp[sizeTemp - 1] != '\n');
        temp[sizeTemp - 1] = '\0';

        stdErr = parseSystemCommand((char*)temp, &cmd, settings);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }
        #ifdef USE_INTERRUPTS
        IRQ_DISABLE;
        #endif /* USE_INTERRUPTS */

        stdErr = fifo_push_C(settings->outComm->Buff_InputCommands, &cmd, sizeof(SystemCommand));

        #ifdef USE_INTERRUPTS
        IRQ_ENABLE;
        #endif /* USE_INTERRUPTS */
    }

    return stdErr;
}


Std_Err init_manager(DeviceSettings* settings)
{
    Std_Err stdErr;

    stdErr = init_outer_operations(settings->outComm);
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    //Initialize SD Card
    HAL_GPIO_WritePin(SDSPI_CS_GPIO_Port, SDSPI_CS_Pin, GPIO_PIN_SET);

    stdErr = init_operations_SDcard(settings->sd);
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    return stdErr;
}


Std_Err clearAllMotorsRoundingErrors(DeviceSettings *settings)
{
    Std_Err stdErr = STD_OK;

    settings->motors[0]->data.err.moveError = 0.0;
    settings->motors[0]->data.err.speedError = 0.0;
    settings->motors[1]->data.err.moveError = 0.0;
    settings->motors[1]->data.err.speedError = 0.0;
    settings->motors[2]->data.err.moveError = 0.0;
    settings->motors[2]->data.err.speedError = 0.0;
    settings->motors[3]->data.err.moveError = 0.0;
    settings->motors[3]->data.err.speedError = 0.0;

    return stdErr;
}


Std_Err getMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns)
{
    Std_Err stdErr = STD_OK;

    MotorData_EEPROM data;
    stdErr = EEPROM_readData(memSettigns, motSettings->device.eepromDataAddress, (uint8_t*)(&data), sizeof(MotorData_EEPROM));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    motSettings->device.stepSize = data.stepSize;
    motSettings->device.maxSpeed = data.maxSpeed;
    motSettings->device.positionZero = data.positionZero;
    motSettings->device.positionEnd = data.positionEnd;

    return stdErr;
}


Std_Err setMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns, MotorData_EEPROM *data)
{
    Std_Err stdErr = STD_OK;

    stdErr = EEPROM_writeData(memSettigns, motSettings->device.eepromDataAddress, (uint8_t*)data, 
        sizeof(MotorData_EEPROM));

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

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

Std_Err _execute_outer_command(DeviceSettings* settings)
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


Std_Err _parse_outer_data(DeviceSettings* settings)
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
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void execute_step(DeviceSettings* settings)
{
    _parse_outer_data(&printerSettings);
    _execute_outer_command(&printerSettings);
    send_outer_command(printerSettings.outComm);

    //SDcard Commands
    parse_data_SDcard(printerSettings.sd);
    execute_command_SDcard(&printerSettings);

    #ifdef LOG_ENABLE
    send_logs_SDcard();
    #endif

    reset_commands_SDcard(printerSettings.sd);
    detecting_endCommand_SDcard(&printerSettings);
}


/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

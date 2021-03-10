/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Manager_Communication
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Manager_Communication.h"
#include <string.h>
#include "Command_Parser.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

SystemCommand executingCmd;
Communication_Flags communication_flags;
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err init_communication_manager(BuffCommunication_Settings* settings, UART_HandleTypeDef* huart)
{
    Std_Err stdErr;

    communication_flags.eofRecieved = false;
    communication_flags.end_program = false;
    communication_flags.executing_program = false;
    communication_flags.executing_command = false;

    init_SystemCommandsParser();
    stdErr = init_buffered_communication(settings, huart);

    return stdErr;
}


Std_Err parse_communication_command(BuffCommunication_Settings* settings)
{
    Std_Err stdErr = STD_OK;

    SystemCommand cmd;
    uint8_t sizeTemp = 0;
    uint8_t* data;
    uint8_t temp[25];

    if(settings->EOL_recieved)
    {
        settings->EOL_recieved = false;

        do
        {
            stdErr = fifo_front(settings->Buff_IN, (void**)&data);
            temp[sizeTemp++] = *data;
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            stdErr = fifo_pop_C(settings->Buff_IN);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

        }while(temp[sizeTemp - 1] != '\n');
        temp[sizeTemp - 1] = '\0';

        stdErr = parse_SystemCommand((char*)temp, &cmd);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }

        #ifdef USE_INTERRUPTS
        IRQ_DISABLE;
        #endif /* USE_INTERRUPTS */

        stdErr = fifo_push_C(settings->Buff_InputCommands, &cmd, sizeof(SystemCommand));

        #ifdef USE_INTERRUPTS
        IRQ_ENABLE;
        #endif /* USE_INTERRUPTS */
    }

    return stdErr;
}


Std_Err execute_communication_command(BuffCommunication_Settings* settings, bool motors_state)
{
    Std_Err stdErr = STD_OK;

    SystemCommand* cmd = NULL;
    uint8_t listSize;


    if(communication_flags.executing_program)
    {
        listSize = fifo_getSize(settings->Buff_InputCommands);

        /* no command ongoing, initialize new command */
        if(listSize > 0 && !communication_flags.executing_command)
        {
            stdErr = fifo_front(settings->Buff_InputCommands, (void**)&cmd);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            memcpy(&executingCmd, cmd, sizeof(SystemCommand));
            stdErr = fifo_pop_C(settings->Buff_InputCommands);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            stdErr = executingCmd.init(&executingCmd);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            communication_flags.executing_command = true;
        }

        /* there is command ongoing, process next step */
        if(communication_flags.executing_command && executingCmd.step != NULL && motors_state)
        {
            stdErr = executingCmd.step(&executingCmd);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }
        }

        /* there is no next step to process, deinitialize command */
        if(communication_flags.executing_command && executingCmd.step == NULL && motors_state)
        {
            stdErr = executingCmd.delete(&executingCmd);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            communication_flags.executing_command = false;
        }
    }

    return STD_OK;
}


Std_Err send_communication_command(BuffCommunication_Settings* settings)
{
    return send_buffered_message(settings);
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

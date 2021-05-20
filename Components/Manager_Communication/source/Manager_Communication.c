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
#include "Project_Config.h"
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

void init_communication_manager(Communication_Settings* settings,
    BuffCommunication_Settings* buff_comm, Motor** motors, EEPROMSettings* eeprom,
    SDCard_Settings* sd, BoundariesDetector_Settings* boundaryDetection,
    uint8_t* motor_data_addresses)
{
    settings->buff_comm = buff_comm;
    settings->flags.executing_command = false;

    init_SystemCommandsParser(&(settings->sys_comm), buff_comm, motors, eeprom, sd,
        boundaryDetection, motor_data_addresses);
}


Std_Err parse_communication_command(Communication_Settings* settings)
{
    Std_Err stdErr = STD_OK;

    SystemCommand cmd;
    uint8_t sizeTemp = 0;
    uint8_t* data;
    uint8_t temp[25];

    if(settings->buff_comm->EOL_recieved)
    {
        settings->buff_comm->EOL_recieved = false;

        do
        {
            stdErr = fifo_front(settings->buff_comm->Buff_IN, (void**)&data);
            temp[sizeTemp++] = *data;
            if(stdErr != STD_OK) { return stdErr; }

            stdErr = fifo_pop_C(settings->buff_comm->Buff_IN);
            if(stdErr != STD_OK) { return stdErr; }

        }while(temp[sizeTemp - 1] != '\n');
        temp[sizeTemp - 1] = '\0';

        stdErr = parse_SystemCommand(&(settings->sys_comm), (char*)temp, &cmd);
        if(stdErr != STD_OK) { return stdErr; }

        #ifdef USE_INTERRUPTS
        IRQ_DISABLE;
        #endif /* USE_INTERRUPTS */

        stdErr = fifo_push_C(settings->buff_comm->Buff_InputCommands, &cmd, sizeof(SystemCommand));

        #ifdef USE_INTERRUPTS
        IRQ_ENABLE;
        #endif /* USE_INTERRUPTS */
    }

    return stdErr;
}


Std_Err execute_communication_command(Communication_Settings* settings)
{
    Std_Err stdErr = STD_OK;

    SystemCommand* cmd = NULL;
    uint8_t listSize;

    listSize = fifo_getSize(settings->buff_comm->Buff_InputCommands);

    /* no command ongoing, initialize new command */
    if(listSize > 0 && !settings->flags.executing_command)
    {
        stdErr = fifo_front(settings->buff_comm->Buff_InputCommands, (void**)&cmd);
        if(stdErr != STD_OK) { return stdErr; }

        memcpy(&(settings->executingCmd), cmd, sizeof(SystemCommand));
        stdErr = fifo_pop_C(settings->buff_comm->Buff_InputCommands);
        if(stdErr != STD_OK) { return stdErr; }

        stdErr = settings->executingCmd.init(&(settings->sys_comm), &(settings->executingCmd));
        if(stdErr != STD_OK) { return stdErr; }

        settings->flags.executing_command = true;
    }

    /* there is command ongoing, process next step */
    if(settings->flags.executing_command && settings->executingCmd.step != NULL)
    {
        stdErr = settings->executingCmd.step(&(settings->sys_comm), &(settings->executingCmd));
        if(stdErr != STD_OK) { return stdErr; }
    }

    /* there is no next step to process, deinitialize command */
    if(settings->flags.executing_command && settings->executingCmd.step == NULL)
    {
        if(settings->executingCmd.remove != NULL)
        {
            stdErr = settings->executingCmd.remove(&(settings->sys_comm),
                &(settings->executingCmd));
            if(stdErr != STD_OK) { return stdErr; }
        }

        settings->flags.executing_command = false;
    }

    return stdErr;
}


Std_Err send_communication_command(Communication_Settings* settings)
{
    Std_Err stdErr;

    stdErr = send_buffered_message(settings->buff_comm);
    if(stdErr == STD_BUSY_ERROR) { stdErr = STD_OK; }

    return stdErr;
}


Std_Err send_message(Communication_Settings* settings, char* msg, uint8_t msgSize)
{
    Std_Err stdErr;

    stdErr = add_message_to_send(settings->buff_comm, msg, msgSize);
    if(stdErr == STD_BUSY_ERROR) { stdErr = STD_OK; }

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Manger_SDcard
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Manager_SDcard.h"
#include <string.h>
#include <stdio.h>
#include "GCode_Parser.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

FATFS fatfs;
FIL file;
GCodeCommand executingCmd;
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err init_manager_SDcard(SDCard_Settings* settings, Motor** motors)
{
    Std_Err stdErr = STD_OK;

    settings->fatfs = &fatfs;
    settings->file = &file;

    settings->flags.end_program = false;
    settings->flags.eofRecieved = false;
    settings->flags.executing_program = false;
    settings->flags.executing_command = false;

    settings->activeTab = 1;
    settings->unactiveTab = 0;
    settings->counterTab[0] = BYTES_TO_READ;
    settings->counterTab[1] = BYTES_TO_READ;
    settings->cnt = 0;
    settings->bytesRead = 0;

    stdErr = fifo_create(&(settings->BuffIN_SDcmd));
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    init_GCodeParser(motors);
    return stdErr;
}


Std_Err parse_command_SDcard(SDCard_Settings* settings)
{
    Std_Err stdErr = STD_OK;

    uint8_t listSize;

    listSize = fifo_getSize(settings->BuffIN_SDcmd);

    while(!settings->flags.end_program && listSize < 5 &&
        settings->flags.executing_program)
    {
        if(settings->cnt >= settings->bytesRead)
        {
            if(settings->flags.eofRecieved)
            {
                settings->flags.end_program = true;
                /*TODO: check if this return value is correct*/
                return STD_OK;
            }

            settings->activeTab = settings->activeTab ? 0 : 1;
            settings->unactiveTab = settings->activeTab ? 0 : 1;

            f_read(settings->file, settings->dataSDin[settings->activeTab], BYTES_TO_READ,
                    &(settings->bytesRead));

            if(BYTES_TO_READ > settings->bytesRead)
            {
                settings->flags.eofRecieved = true;
            }

            settings->counterTab[settings->activeTab] = 0;
            settings->cnt = 0;
        }

        while(settings->cnt < settings->bytesRead &&
                settings->dataSDin[settings->activeTab][settings->cnt] != '\n')
        {
            ++(settings->cnt);
        }

        if(settings->dataSDin[settings->activeTab][settings->cnt] == '\n')
        {
            uint8_t cmdData[BYTES_TO_READ];
            uint8_t cmdLen = 0;

            if(settings->counterTab[settings->unactiveTab] < BYTES_TO_READ)
            {
                cmdLen = BYTES_TO_READ - settings->counterTab[settings->unactiveTab];

                memcpy(cmdData, settings->dataSDin[settings->unactiveTab] +
                       settings->counterTab[settings->unactiveTab], cmdLen);

                settings->counterTab[settings->unactiveTab] += cmdLen;
            }

            memcpy(cmdData + cmdLen, settings->dataSDin[settings->activeTab] +
                   settings->counterTab[settings->activeTab], settings->cnt);

            cmdLen += settings->cnt - settings->counterTab[settings->activeTab];
            settings->counterTab[settings->activeTab] = settings->cnt + 1;

            cmdData[cmdLen - 1] = '\0';

            GCodeCommand cmd;
            stdErr = parse_GCodeCommand((char*)cmdData, &cmd);

            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            stdErr = fifo_push_C(settings->BuffIN_SDcmd, &cmd, sizeof(GCodeCommand));
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            ++(settings->cnt);
        }

        listSize = fifo_getSize(settings->BuffIN_SDcmd);
    }

    return stdErr;
}


Std_Err execute_command_SDcard(SDCard_Settings* settings, bool motors_state)
{
    Std_Err stdErr;

    GCodeCommand* cmd = NULL;
    uint8_t listSize;


    if(settings->flags.executing_program)
    {
        listSize = fifo_getSize(settings->BuffIN_SDcmd);

        /* no command ongoing, initialize new command */
        if(listSize > 0 && !settings->flags.executing_command)
        {
            stdErr = fifo_front(settings->BuffIN_SDcmd, (void**)&cmd);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            memcpy(&executingCmd, cmd, sizeof(GCodeCommand));
            stdErr = fifo_pop_C(settings->BuffIN_SDcmd);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            stdErr = executingCmd.init(&executingCmd);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            settings->flags.executing_command = true;
        }

        /* there is command ongoing, process next step */
        if(settings->flags.executing_command && executingCmd.step != NULL && motors_state)
        {
            stdErr = executingCmd.step(&executingCmd);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }
        }

        /* there is no next step to process, deinitialize command */
        if(settings->flags.executing_command && executingCmd.step == NULL && motors_state)
        {
            stdErr = executingCmd.remove(&executingCmd);
            if(stdErr != STD_OK)
            {
                return stdErr;
            }

            settings->flags.executing_command = false;
        }
    }

    return STD_OK;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

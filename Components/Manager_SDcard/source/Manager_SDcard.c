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

#define MSG_FR_OK ("FR_OK")
#define MSG_FR_DISK_ERR ("FR_DISK_ERR")
#define MSG_FR_INT_ERR ("FR_INT_ERR")
#define MSG_FR_NOT_READY ("FR_NOT_READY")
#define MSG_FR_NO_FILE ("FR_NO_FILE")
#define MSG_FR_NO_PATH ("FR_NO_PATH")
#define MSG_FR_INVALID_NAME ("FR_INVALID_NAME")
#define MSG_FR_DENIED ("FR_DENIED")
#define MSG_FR_EXIST ("FR_EXIST")
#define MSG_FR_INVALID_OBJECT ("FR_INVALID_OBJECT")
#define MSG_FR_WRITE_PROTECTED ("FR_WRITE_PROTECTED")
#define MSG_FR_INVALID_DRIVE ("FR_INVALID_DRIVE")
#define MSG_FR_NOT_ENABLED ("FR_NOT_ENABLED")
#define MSG_FR_NO_FILESYSTEM ("FR_NO_FILESYSTEM")
#define MSG_FR_MKFS_ABORTED ("FR_MKFS_ABORTED")
#define MSG_FR_TIMEOUT ("FR_TIMEOUT")
#define MSG_FR_LOCKED ("FR_LOCKED")
#define MSG_FR_NOT_ENOUGH_CORE ("FR_NOT_ENOUGH_CORE")
#define MSG_FR_TOO_MANY_OPEN_FILES ("FR_TOO_MANY_OPEN_FILES")
#define MSG_FR_INVALID_PARAMETER ("FR_INVALID_PARAMETER")
#define MSG_FR_UNKNOWN ("FR_UNKNOWN_ERROR")
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

FATFS fatfs;
FIL file;
GCodeCommand executingCmdSD;
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Err_Msg translate_error_sdcard_to_project(FRESULT sdStatus)
{
    Err_Msg retVal;

    switch(sdStatus)
    {
        case FR_OK:
            retVal.err = STD_OK; retVal.msg = MSG_FR_OK; break;
        case FR_DISK_ERR:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_DISK_ERR; break;
        case FR_INT_ERR:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_INT_ERR; break;
        case FR_NOT_READY:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_NOT_READY; break;
        case FR_NO_FILE:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_NO_FILE; break;
        case FR_NO_PATH:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_NO_PATH; break;
        case FR_INVALID_NAME:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_INVALID_NAME; break;
        case FR_DENIED:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_DENIED; break;
        case FR_EXIST:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_EXIST; break;
        case FR_INVALID_OBJECT:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_INVALID_OBJECT; break;
        case FR_WRITE_PROTECTED:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_WRITE_PROTECTED; break;
        case FR_INVALID_DRIVE:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_INVALID_DRIVE; break;
        case FR_NOT_ENABLED:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_NOT_ENABLED; break;
        case FR_NO_FILESYSTEM:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_NO_FILESYSTEM; break;
        case FR_MKFS_ABORTED:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_MKFS_ABORTED; break;
        case FR_TIMEOUT:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_TIMEOUT; break;
        case FR_LOCKED:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_LOCKED; break;
        case FR_NOT_ENOUGH_CORE:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_NOT_ENOUGH_CORE; break;
        case FR_TOO_MANY_OPEN_FILES:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_TOO_MANY_OPEN_FILES; break;
        case FR_INVALID_PARAMETER:
            retVal.err = STD_IO_ERROR; retVal.msg = MSG_FR_INVALID_PARAMETER; break;
        default:
            retVal.err = STD_ERROR; retVal.msg = MSG_FR_UNKNOWN; break;
    }
    return retVal;
}


Std_Err init_manager_SDcard(SDCard_Settings* settings, Motor** motors,
    BuffCommunication_Settings* buff_comm, bool* motors_are_on)
{
    FRESULT sdErr;
    Err_Msg msgErr;
    Std_Err stdErr = STD_OK;

    settings->fatfs = &fatfs;
    settings->file = &file;
    sdErr = f_mount(settings->fatfs, "", 0);
    msgErr = translate_error_sdcard_to_project(sdErr);
    if(msgErr.err != STD_OK) { return msgErr.err; }

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
    if(stdErr != STD_OK) { return stdErr; }

    init_GCodeParser(motors, buff_comm, motors_are_on);
    return stdErr;
}


Std_Err parse_command_SDcard(SDCard_Settings* settings)
{
    Std_Err stdErr = STD_OK;
    Err_Msg msgErr;
    FRESULT sdErr;

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

            sdErr = f_read(settings->file, settings->dataSDin[settings->activeTab], BYTES_TO_READ,
                    &(settings->bytesRead));
            msgErr = translate_error_sdcard_to_project(sdErr);
            if(msgErr.err != STD_OK) { return msgErr.err; }
            stdErr = msgErr.err;

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

            if(stdErr != STD_OK) { return stdErr; }

            stdErr = fifo_push_C(settings->BuffIN_SDcmd, &cmd, sizeof(GCodeCommand));
            if(stdErr != STD_OK) { return stdErr; }

            ++(settings->cnt);
        }

        listSize = fifo_getSize(settings->BuffIN_SDcmd);
    }

    return stdErr;
}


Std_Err execute_command_SDcard(SDCard_Settings* settings)
{
    Std_Err stdErr = STD_OK;

    GCodeCommand* cmd = NULL;
    uint8_t listSize;


    if(settings->flags.executing_program)
    {
        listSize = fifo_getSize(settings->BuffIN_SDcmd);

        if(settings->flags.end_program && listSize == 0 && !settings->flags.executing_command)
        {
            settings->flags.executing_program = false;
            settings->flags.end_program = false;
            settings->flags.eofRecieved = false;
            return STD_OK;
        }

        /* no command ongoing, initialize new command */
        if(listSize > 0 && !settings->flags.executing_command)
        {
            stdErr = fifo_front(settings->BuffIN_SDcmd, (void**)&cmd);
            if(stdErr != STD_OK) { return stdErr; }

            memcpy(&executingCmdSD, cmd, sizeof(GCodeCommand));
            stdErr = fifo_pop_C(settings->BuffIN_SDcmd);
            if(stdErr != STD_OK) { return stdErr; }

            stdErr = executingCmdSD.init(&executingCmdSD);
            if(stdErr != STD_OK) { return stdErr; }

            settings->flags.executing_command = true;
        }

        /* there is command ongoing, process next step */
        if(settings->flags.executing_command && executingCmdSD.step != NULL)
        {
            stdErr = executingCmdSD.step(&executingCmdSD);
            if(stdErr != STD_OK) { return stdErr; }
        }

        /* there is no next step to process, deinitialize command */
        if(settings->flags.executing_command && executingCmdSD.step == NULL)
        {
            if(executingCmdSD.remove != NULL)
            {
                stdErr = executingCmdSD.remove(&executingCmdSD);
                if(stdErr != STD_OK) { return stdErr; }
            }

            settings->flags.executing_command = false;
        }
    }

    return stdErr;
}


Err_Msg sdcard_open_file(SDCard_Settings* settings, const char* file_name, BYTE mode)
{
    FRESULT sdErr;
    Err_Msg msgErr;

    sdErr = f_open(settings->file, file_name, mode);
    msgErr = translate_error_sdcard_to_project(sdErr);

    return msgErr;
}


Err_Msg sdcard_close_file(SDCard_Settings* settings)
{
    FRESULT sdErr;
    Err_Msg msgErr;

    sdErr = f_close(settings->file);
    msgErr = translate_error_sdcard_to_project(sdErr);

    return msgErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

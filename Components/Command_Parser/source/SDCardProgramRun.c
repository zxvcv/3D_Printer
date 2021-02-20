/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Command_Parser
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "_commands.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

Std_Err systemCmd_SDCardProgramRun(SystemCommand* cmd, DeviceSettings* settings)
{
    Std_Err stdErr = STD_OK;
    /*TODO: distinguishing between errors*/
    /*TODO: add forwarding SDcard error*/
    f_mount(settings->fatfs, "", 0);
    f_open(settings->sd->file, "fl.txt", FA_READ);

    #ifdef LOG_ENABLE
    #include "manager.h"
    extern FIL logFile;
    f_open(&logFile, "logs.txt", FA_CREATE_ALWAYS | FA_WRITE);
    UINT writeSize;
    f_write(&logFile, "[START]\r\n", 9, &writeSize);
    f_sync(&logFile);
    #endif

    settings->sd->executing_SDprogram = true;
    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

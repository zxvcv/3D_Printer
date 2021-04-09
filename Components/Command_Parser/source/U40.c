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

Std_Err step_U40(SystemCommand* cmd)
{
    Std_Err stdErr;
    stdErr = STD_OK;

    stdErr = parse_command_SDcard(global_systemCmd_settings.sd);
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = execute_command_SDcard(global_systemCmd_settings.sd);

    return stdErr;
}


Std_Err remove_U40(SystemCommand* cmd)
{
    //f_close

    add_message_to_send(global_systemCmd_settings.buff_comm, "$>remove_U40\n", 13); // DEBUG
    return STD_OK;
}


Std_Err init_U40(SystemCommand* cmd)
{
    Std_Err stdErr;
    stdErr = STD_OK;
    cmd->remove = remove_U40;
    cmd->step = step_U40;

    f_mount(global_systemCmd_settings.sd->fatfs, "", 0);
    f_open(global_systemCmd_settings.sd->file, "fl.txt", FA_READ);

    stdErr = parse_command_SDcard(global_systemCmd_settings.sd);

    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

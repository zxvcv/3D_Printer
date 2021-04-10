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

    stdErr = parse_command_SDcard(global_systemCmd_settings.sd);
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = execute_command_SDcard(global_systemCmd_settings.sd);
    if(stdErr != STD_OK) { return stdErr; }

    if(!global_systemCmd_settings.sd->flags.executing_program)
    {
        cmd->step = NULL;
    }

    return stdErr;
}


Std_Err remove_U40(SystemCommand* cmd)
{
    add_message_to_send(global_systemCmd_settings.buff_comm, "$>remove_U40\n", 13); // DEBUG
    Err_Msg msgErr;
    msgErr = sdcard_close_file(global_systemCmd_settings.sd);

    return msgErr.err;
}


Std_Err init_U40(SystemCommand* cmd)
{
    add_message_to_send(global_systemCmd_settings.buff_comm, "$>init_U40\n", 11); // DEBUG
    Std_Err stdErr;
    Err_Msg msgErr;

    stdErr = STD_OK;
    cmd->remove = remove_U40;
    cmd->step = step_U40;

    msgErr = sdcard_open_file(global_systemCmd_settings.sd, "fl.txt", FA_READ);
    if(msgErr.err != STD_OK) { return msgErr.err; }

    global_systemCmd_settings.sd->flags.executing_program = true;

    stdErr = parse_command_SDcard(global_systemCmd_settings.sd);

    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

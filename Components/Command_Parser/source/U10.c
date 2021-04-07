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

Std_Err init_U10(SystemCommand* cmd)
{
    Std_Err stdErr;
    char temp[15];

    // prepare GCode command for set absolute movement
    cmd->remove = remove_forward_GCode;
    cmd->step = step_forward_GCode;

    sprintf(global_systemCmd_settings.msg_buff, "G90");
    stdErr = parse_GCodeCommand(global_systemCmd_settings.msg_buff, &(cmd->gcode_cmd));
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = cmd->gcode_cmd.init(&(cmd->gcode_cmd));
    if(stdErr != STD_OK) { return stdErr; }

    while(cmd->step != NULL)
    {
        stdErr = cmd->step(cmd);
        if(stdErr != STD_OK) { return stdErr; }
    }

    stdErr = cmd->remove(cmd);
    if(stdErr != STD_OK) { return stdErr; }


    // prepare GCode command for movement
    cmd->remove = remove_forward_GCode;
    cmd->step = step_forward_GCode;

    sprintf(global_systemCmd_settings.msg_buff, "G1");

    for(int i=0x01; i<=PARAM_LAST; i<<=1)
    {
        if(cmd->used_fields & i)
        {
            switch(i)
            {
                case PARAM_X: sprintf(temp, " %c%f", 'X', cmd->data.x); break;
                case PARAM_Y: sprintf(temp, " %c%f", 'Y', cmd->data.y); break;
                case PARAM_Z: sprintf(temp, " %c%f", 'Z', cmd->data.z); break;
                case PARAM_E: sprintf(temp, " %c%f", 'E', cmd->data.e); break;
                case PARAM_F: sprintf(temp, " %c%f", 'F', cmd->data.f); break;
                default: break;
            }

            strcat(global_systemCmd_settings.msg_buff, temp);
        }
    }

    strcat(global_systemCmd_settings.msg_buff, "\n");
    stdErr = parse_GCodeCommand(global_systemCmd_settings.msg_buff, &(cmd->gcode_cmd));
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = cmd->gcode_cmd.init(&(cmd->gcode_cmd));

    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

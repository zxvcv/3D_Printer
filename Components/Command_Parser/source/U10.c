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
    char temp[15];

    // prepare GCode command for set absolute movement
    cmd->remove = remove_forward_GCode;
    cmd->step = step_forward_GCode;

    uint8_t msgSize = sprintf(global_systemCmd_settings->msg_buff, "G90\n");
    stdErr = parse_GCodeCommand(&(global_systemCmd_settings->msg_buff), &(cmd->gcode_cmd));
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = cmd->gcode_cmd.init(cmd->gcode_cmd);
    if(stdErr != STD_OK) { return stdErr; }

    while(cmd->gcode_cmd.step != NULL)
    {
        stdErr = cmd->gcode_cmd.step(cmd->gcode_cmd);
        if(stdErr != STD_OK) { return stdErr; }
    }

    stdErr = cmd->gcode_cmd.remove(cmd->gcode_cmd);
    if(stdErr != STD_OK) { return stdErr; }


    // prepare GCode command for movement
    cmd->remove = remove_forward_GCode;
    cmd->step = step_forward_GCode;

    global_systemCmd_settings->msg_buff = "G1"

    for(int i=0x01; i<=PARAM_LAST; i<<=1)
    {
        if(cmd->used_fields & i)
        {
            switch(i)
            {
                case PARAM_X: sprintf(temp, "%c%f", 'X', cmd->data.x); break;
                case PARAM_Y: sprintf(temp, "%c%f", 'Y', cmd->data.y); break;
                case PARAM_Z: sprintf(temp, "%c%f", 'Z', cmd->data.z); break;
                case PARAM_E: sprintf(temp, "%c%f", 'E', cmd->data.e); break;
                case PARAM_F: sprintf(temp, "%c%f", 'F', cmd->data.f); break;
                default: break;
            }

            uint8_t msgSize = sprintf(global_systemCmd_settings->msg_buff,
                "%s %s",
                global_systemCmd_settings->msg_buff,
                temp);
        }
    }

    uint8_t msgSize = sprintf(global_systemCmd_settings->msg_buff,
        "%s\n", global_systemCmd_settings->msg_buff);

    stdErr = parse_GCodeCommand(&(global_systemCmd_settings->msg_buff), &(cmd->gcode_cmd));
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = cmd->gcode_cmd.init(cmd->gcode_cmd);

    return STD_OK;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

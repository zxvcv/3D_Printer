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

Std_Err init_U12(SystemCommand_Settings* settings, SystemCommand* cmd)
{
    Std_Err stdErr;
    char temp[15];

    // prepare GCode command for set absolute movement
    cmd->remove = remove_forward_GCode;
    cmd->step = step_forward_GCode;

    sprintf(settings->msg_buff, "G17");
    stdErr = parse_GCodeCommand(&(settings->sd->gcode), settings->msg_buff, &(cmd->gcode_cmd));
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = cmd->gcode_cmd.init(&(settings->sd->gcode), &(cmd->gcode_cmd));
    if(stdErr != STD_OK) { return stdErr; }

    while(cmd->step != NULL)
    {
        stdErr = cmd->step(settings, cmd);
        if(stdErr != STD_OK) { return stdErr; }
    }

    stdErr = cmd->remove(settings, cmd);
    if(stdErr != STD_OK) { return stdErr; }

    // prepare GCode command for movement
    cmd->remove = remove_forward_GCode;
    cmd->step = step_forward_GCode;

    sprintf(settings->msg_buff, "G2");

    for(int i=PARAM_X; i<=PARAM_K; i<<=1)
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
                case PARAM_I: sprintf(temp, " %c%f", 'I', cmd->data.i); break;
                case PARAM_J: sprintf(temp, " %c%f", 'J', cmd->data.j); break;
                case PARAM_K: sprintf(temp, " %c%f", 'K', cmd->data.k); break;
                default: break;
            }

            strcat(settings->msg_buff, temp);
        }
    }

    strcat(settings->msg_buff, "\n");
    stdErr = parse_GCodeCommand(&(settings->sd->gcode), settings->msg_buff, &(cmd->gcode_cmd));
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = cmd->gcode_cmd.init(&(settings->sd->gcode), &(cmd->gcode_cmd));

    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

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

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err step_forward_GCode(SystemCommand_Settings* settings, SystemCommand* cmd)
{
    Std_Err stdErr = STD_OK;

    if(cmd->gcode_cmd.step != NULL)
    {
        stdErr = cmd->gcode_cmd.step(&(settings->sd->gcode), &(cmd->gcode_cmd));
    }
    else
    {
        cmd->step = NULL;
    }

    return stdErr;
}


Std_Err remove_forward_GCode(SystemCommand_Settings* settings, SystemCommand* cmd)
{
    Std_Err stdErr = STD_OK;

    if(cmd->gcode_cmd.remove != NULL)
    {
        stdErr = cmd->gcode_cmd.remove(&(settings->sd->gcode), &(cmd->gcode_cmd));
    }

    return stdErr;
}


Std_Err forward_command_concurrently(SystemCommand_Settings* settings, SystemCommand* cmd,
    char* cmd_name, bool forward_parameters)
{
    Std_Err stdErr;

    cmd->remove = remove_forward_GCode;
    cmd->step = step_forward_GCode;

    sprintf(settings->msg_buff, cmd_name);

    if(forward_parameters)
    {
        char temp[15];

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
    }

    stdErr = parse_GCodeCommand(&(settings->sd->gcode), settings->msg_buff, &(cmd->gcode_cmd));
    if(stdErr != STD_OK) { return stdErr; }

    stdErr = cmd->gcode_cmd.init(&(settings->sd->gcode), &(cmd->gcode_cmd));

    return stdErr;
}


Std_Err forward_command_immediately(SystemCommand_Settings* settings, SystemCommand* cmd,
    char* cmd_name, bool forward_parameters)
{
    Std_Err stdErr;

    stdErr = forward_command_concurrently(settings, cmd, cmd_name, forward_parameters);
    if(stdErr != STD_OK) { return stdErr; }

    while(cmd->step != NULL)
    {
        stdErr = cmd->step(settings, cmd);
        if(stdErr != STD_OK) { return stdErr; }
    }

    stdErr = cmd->remove(settings, cmd);
    if(stdErr != STD_OK) { return stdErr; }

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

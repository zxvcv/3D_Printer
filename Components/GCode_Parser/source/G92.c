/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: GCode_Parser
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

Std_Err init_G92(GCodeCommand* cmd)
{
    cmd->delete = NULL;
    cmd->step = NULL;

    Motor* motors = global_gcode_settings.motors;

    Std_Err stdErr;

    if(cmd->used_fields & PARAM_X)
    {
        stdErr = motor_set_position(&motors[MOTOR_X], cmd->data.x);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }
    }

    if(cmd->used_fields & PARAM_Y)
    {
        stdErr = motor_set_position(&motors[MOTOR_Y], cmd->data.y);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }
    }

    if(cmd->used_fields & PARAM_Z)
    {
        stdErr = motor_set_position(&motors[MOTOR_Z], cmd->data.z);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }
    }

    return STD_OK;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

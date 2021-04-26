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

Std_Err init_U26(SystemCommand_Settings* settings, SystemCommand* cmd)
{
    Std_Err stdErr = STD_OK;

    cmd->remove = NULL;
    cmd->step = NULL;

    if((cmd->used_fields & PARAM_X) && (cmd->used_fields & PARAM_Y))
    {
        settings->sd->gcode.plane_selection.plane_x = 1;
        settings->sd->gcode.plane_selection.plane_y = 1;
        settings->sd->gcode.plane_selection.plane_z = 0;
    }
    else if((cmd->used_fields & PARAM_X) && (cmd->used_fields & PARAM_Z))
    {
        settings->sd->gcode.plane_selection.plane_x = 1;
        settings->sd->gcode.plane_selection.plane_y = 0;
        settings->sd->gcode.plane_selection.plane_z = 1;
    }
    else if((cmd->used_fields & PARAM_Y) && (cmd->used_fields & PARAM_Z))
    {
        settings->sd->gcode.plane_selection.plane_x = 0;
        settings->sd->gcode.plane_selection.plane_y = 1;
        settings->sd->gcode.plane_selection.plane_z = 1;
    }
    else
    {
        settings->sd->gcode.plane_selection.plane_x = 0;
        settings->sd->gcode.plane_selection.plane_y = 0;
        settings->sd->gcode.plane_selection.plane_z = 0;
        stdErr = STD_PARAMETER_ERROR;
    }

    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

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

Std_Err init_U03(SystemCommand_Settings* settings, SystemCommand* cmd)
{
    Std_Err stdErr = STD_OK;
    cmd->remove = NULL;
    cmd->step = NULL;

    uint8_t msgSize = sprintf(settings->msg_buff,
        "%cU03 %f %f %u %u %u%u%u\n",
        '3',
        settings->sd->gcode.speed,
        settings->sd->gcode.angle_step,
        settings->sd->gcode.positioning_mode,
        settings->sd->gcode.circle_move_mode,
        settings->sd->gcode.plane_selection.plane_x,
        settings->sd->gcode.plane_selection.plane_y,
        settings->sd->gcode.plane_selection.plane_z);

    stdErr = add_message_to_send(settings->buff_comm,
        settings->msg_buff, msgSize);
    if(stdErr == STD_BUSY_ERROR) { stdErr = STD_OK; }
    if(stdErr != STD_OK) { return stdErr; }

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

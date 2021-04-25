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

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

extern Std_Err step_G2(GCode_Settings* settings, GCodeCommand* cmd);
extern Std_Err remove_G2(GCode_Settings* settings, GCodeCommand* cmd);
extern Std_Err init_circle_movement(GCode_Settings* settings, GCodeCommand* cmd);


Std_Err init_G3(GCode_Settings* settings, GCodeCommand* cmd)
{
    Std_Err stdErr = STD_OK;

    cmd->remove = remove_G2;
    cmd->step = step_G2;

    // TODO: parametrize angle step
    settings->angle_step = 10.;
    settings->circle_move_mode = COUNTER_CLOCKWISE_CIRCLE;

    stdErr = init_circle_movement(settings, cmd);

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

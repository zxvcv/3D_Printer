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

Std_Err command_G92(GCodeCommand* cmd, DeviceSettings* settings)
{
    /*TODO: check gCode state before start executing command*/
    Std_Err stdErr= STD_OK;

    if(cmd->usedFields._x == 1)
    {
        /*TODO: improved accuracy*/
        settings->motors[MOTOR_X]->data.position = cmd->data.x * ACCURACY;
    }

    if(cmd->usedFields._y == 1)
    {
        /*TODO: improved accuracy*/
        settings->motors[MOTOR_Y]->data.position = cmd->data.y * ACCURACY;
    }
        
    if(cmd->usedFields._z == 1)
    {
        /*TODO: improved accuracy*/
        settings->motors[MOTOR_Z1]->data.position = cmd->data.z * ACCURACY;
        settings->motors[MOTOR_Z2]->data.position = cmd->data.z * ACCURACY;
    }

    /*TODO: write implementation for extruder*/
    /*if(cmd->usedFields._e == 1)
    {
        //set extruder positoin
    }*/
        
    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

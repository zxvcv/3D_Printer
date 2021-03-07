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

Std_Err step_G1(GCodeCommand* cmd)
{
    Std_Err stdErr;
    Motor* motors = global_gcode_settings.motors;
    vect3D_d move;

    move.x = cmd->target_position.x - motors[MOTOR_X].data.position;
    move.y = cmd->target_position.y - motors[MOTOR_Y].data.position;
    move.z = cmd->target_position.z - motors[MOTOR_Z].data.position;

    if(cmd->used_fields & PARAM_F)
    {
        global_gcode_settings.speed = cmd->data.f;
    }

    vect3D_d velocity = getVelocity3D(move, global_gcode_settings.speed);

    MotorCounters counters_val;
    bool direction;

    double move_tab[3] = { move.x, move.y, move.z };
    double velocity_tab[3] = { velocity.x, velocity.y, velocity.z };

    for(int i=MOTOR_X; i<MOTOR_Z ; ++i)
    {
        stdErr = motor_get_linear_move_settings(&motors[i],
                         move_tab[i],
                         velocity_tab[i],
                         ACCURACY, &counters_val, &direction);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }
        motor_set_counters(&motors[i], &counters_val);
        motor_set_direction(&motors[i], direction);
    }

    for(int i=MOTOR_X; i<MOTOR_Z ; ++i)
    {
        stdErr = motor_start(&motors[i]);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }
    }

    cmd->step = NULL;
    return STD_OK;
}


Std_Err init_G1(GCodeCommand* cmd)
{
    cmd->delete = NULL;
    cmd->step = step_G1;

    Motor* motors = global_gcode_settings.motors;

    if(global_gcode_settings.positioning_mode == ABSOLUTE)
    {
        cmd->target_position.x = (cmd->used_fields & PARAM_X ?
            cmd->data.x : motors[MOTOR_X].data.position + motors[MOTOR_X].data.position_error);
        cmd->target_position.y = (cmd->used_fields & PARAM_Y ?
            cmd->data.y : motors[MOTOR_Y].data.position + motors[MOTOR_Y].data.position_error);
        cmd->target_position.z = (cmd->used_fields & PARAM_Z ?
            cmd->data.z : motors[MOTOR_Z].data.position + motors[MOTOR_Z].data.position_error);
    }
    else if(global_gcode_settings.positioning_mode == RELATIVE)
    {
        cmd->target_position.x = motors[MOTOR_X].data.position + motors[MOTOR_X].data.position_error +
            (cmd->used_fields & PARAM_X ? cmd->data.x : 0);
        cmd->target_position.y = motors[MOTOR_Y].data.position + motors[MOTOR_Y].data.position_error +
            (cmd->used_fields & PARAM_Y ? cmd->data.y : 0);
        cmd->target_position.z = motors[MOTOR_Z].data.position + motors[MOTOR_Z].data.position_error +
            (cmd->used_fields & PARAM_Z ? cmd->data.z : 0);
    }

    return STD_OK;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

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

Std_Err init_G1(GCodeCommand* cmd, Motor* motors)
{
    cmd->delete = NULL;
    cmd->step = step_G1;

    if(global_gcode_settings.positioning_mode == ABSOLUTE)
    {
        cmd->target_position.x = (cmd->used_fields & PARAM_X ?
            cmd->data.x : motors[MOTOR_X]->data->position + global_gcode_settings.position_error.x);
        cmd->target_position.y = (cmd->used_fields & PARAM_Y ?
            cmd->data.y : motors[MOTOR_Y]->data->position + global_gcode_settings.position_error.y);
        cmd->target_position.z = (cmd->used_fields & PARAM_Z ?
            cmd->data.z : motors[MOTOR_Z]->data->position + global_gcode_settings.position_error.z);
    }
    else if(global_gcode_settings.positioning_mode == RELATIVE)
    {
        cmd->target_position.x = motors[MOTOR_X]->data->position + global_gcode_settings.position_error.x +
            (cmd->used_fields & PARAM_X ? cmd->data.x : 0)
        cmd->target_position.y = motors[MOTOR_Y]->data->position + global_gcode_settings.position_error.y +
            (cmd->used_fields & PARAM_Y ? cmd->data.y : 0)
        cmd->target_position.z = motors[MOTOR_Z]->data->position + global_gcode_settings.position_error.z +
            (cmd->used_fields & PARAM_Z ? cmd->data.z : 0)
    }

    return STD_OK;
}


Std_Err step_G1(GCodeCommand* cmd, Motor* motors)
{
    Std_Err stdErr;
    vect3D_d move;

    move.x = cmd->target_position.x - motors[MOTOR_X]->data->position;
    move.y = cmd->target_position.y - motors[MOTOR_Y]->data->position;
    move.z = cmd->target_position.z - motors[MOTOR_Z]->data->position;

    if(cmd->used_fields & PARAM_F)
    {
        global_gcode_settings->speed = cmd->data.f;
    }

    vect3D_d velocity = getVelocity3D(move, global_gcode_settings->speed);

    MotorCounters counters_val;
    bool direction;

    for(int i=MOTOR_X; i<MOTOR_Z ; ++i)
    {
        stdErr = motor_get_linear_move_settings(motors[i],
                         (double)*(&move + sizeof(double) * i),
                         (double)*(&velocity + sizeof(double) * i),
                         ACCURACY, &counters_val, &direction);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }
        motor_set_counters(motors[i], &counters_val);
        motor_set_direction(motors[i], direction);
    }

    for(int i=MOTOR_X; i<MOTOR_Z ; ++i)
    {
        stdErr = motor_start(motors[i]);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }
    }

    cmd->step = NULL;
    return STD_OK;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

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

Std_Err step_G28(GCodeCommand* cmd)
{
    add_message_to_send(global_gcode_settings.buff_comm, "$>step_G28\n", 11); // DEBUG
    Std_Err stdErr;
    Motor** motors = global_gcode_settings.motors;
    vect3D_d move;

    move.x = cmd->target_position.x - motors[MOTOR_X]->data.position;
    move.y = cmd->target_position.y - motors[MOTOR_Y]->data.position;
    move.z = cmd->target_position.z - motors[MOTOR_Z]->data.position;

    // ??? is this command supports F parameter?
    if(cmd->used_fields & PARAM_F)
    {
        global_gcode_settings.speed = cmd->data.f;
    }

    vect3D_d velocity = getVelocity3D(move, global_gcode_settings.speed);

    MotorCounters counters_val;
    bool direction;

    double move_tab[3] = { move.x, move.y, move.z };
    double velocity_tab[3] = { velocity.x, velocity.y, velocity.z };

    for(int i=MOTOR_X; i<MOTORS_NUM ; ++i)
    {
        stdErr = motor_get_linear_move_settings(motors[i],
                         move_tab[i],
                         velocity_tab[i],
                         ACCURACY, &counters_val, &direction);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }
        motor_set_counters(motors[i], &counters_val);
        motor_set_direction(motors[i], direction);
    }

    for(int i=MOTOR_X; i<MOTORS_NUM ; ++i)
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


Std_Err init_G28(GCodeCommand* cmd)
{
    add_message_to_send(global_gcode_settings.buff_comm, "$>init_G28\n", 11); // DEBUG

    cmd->remove = NULL;
    cmd->step = step_G28;

    Motor** motors = global_gcode_settings.motors;

    if(cmd->used_fields & PARAM_X)
    {
        cmd->target_position.x = motors[MOTOR_X]->settings.position_zero;
    }
    else
    {
        cmd->target_position.x = motors[MOTOR_X]->data.position +
            motors[MOTOR_X]->data.position_error;
    }

    if(cmd->used_fields & PARAM_Y)
    {
        cmd->target_position.y = motors[MOTOR_Y]->settings.position_zero;
    }
    else
    {
        cmd->target_position.y = motors[MOTOR_Y]->data.position +
            motors[MOTOR_Y]->data.position_error;
    }

    if(cmd->used_fields & PARAM_Z)
    {
        cmd->target_position.z = motors[MOTOR_Z]->settings.position_zero;
    }
    else
    {
        cmd->target_position.z = motors[MOTOR_Z]->data.position +
            motors[MOTOR_Z]->data.position_error;
    }

    return STD_OK;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

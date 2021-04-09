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
    if(*(global_gcode_settings.motors_are_on))
    {
        cmd->step = NULL;
    }

    return STD_OK;
}


Std_Err init_G1(GCodeCommand* cmd)
{
    Std_Err stdErr = STD_OK;

    cmd->remove = NULL;
    cmd->step = step_G1;

    Motor** motors = global_gcode_settings.motors;

    if(global_gcode_settings.positioning_mode == ABSOLUTE)
    {
        cmd->target_position.x = (cmd->used_fields & PARAM_X) ? cmd->data.x :
            (motors[MOTOR_X]->data.position + motors[MOTOR_X]->data.position_error) / ACCURACY;
        cmd->target_position.y = (cmd->used_fields & PARAM_Y) ? cmd->data.y :
            (motors[MOTOR_Y]->data.position + motors[MOTOR_Y]->data.position_error) / ACCURACY;
        cmd->target_position.z = (cmd->used_fields & PARAM_Z) ? cmd->data.z :
            (motors[MOTOR_Z]->data.position + motors[MOTOR_Z]->data.position_error) / ACCURACY;
    }
    else if(global_gcode_settings.positioning_mode == RELATIVE)
    {
        cmd->target_position.x = (cmd->used_fields & PARAM_X ? cmd->data.x : 0) +
            (motors[MOTOR_X]->data.position + motors[MOTOR_X]->data.position_error) / ACCURACY;
        cmd->target_position.y = (cmd->used_fields & PARAM_Y ? cmd->data.y : 0) +
            (motors[MOTOR_Y]->data.position + motors[MOTOR_Y]->data.position_error) / ACCURACY;
        cmd->target_position.z = (cmd->used_fields & PARAM_Z ? cmd->data.z : 0) +
            (motors[MOTOR_Z]->data.position + motors[MOTOR_Z]->data.position_error) / ACCURACY;
    }

    vect3D_d move;

    move.x = cmd->target_position.x - ((double)motors[MOTOR_X]->data.position) / ACCURACY;
    move.y = cmd->target_position.y - ((double)motors[MOTOR_Y]->data.position) / ACCURACY;
    move.z = cmd->target_position.z - ((double)motors[MOTOR_Z]->data.position) / ACCURACY;

    if(cmd->used_fields & PARAM_F)
    {
        global_gcode_settings.speed = cmd->data.f;
    }

    vect3D_d velocity = getVelocity3D(move, global_gcode_settings.speed);

    MotorCounters counters_val;
    bool direction;

    double move_tab[MOTORS_NUM] = { move.x, move.y, move.z , 0. };
    double velocity_tab[MOTORS_NUM] =
        { fabs(velocity.x), fabs(velocity.y), fabs(velocity.z), fabs(0.) };

    for(int i=MOTOR_X; i<MOTORS_NUM ; ++i)
    {
        char temp_buff[100]; // DEBUG
        uint8_t size_temp = sprintf(temp_buff, "$%f %f %d\n",
            move_tab[i], velocity_tab[i], ACCURACY); // DEBUG
        add_message_to_send(global_gcode_settings.buff_comm, temp_buff, size_temp); // DEBUG
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

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

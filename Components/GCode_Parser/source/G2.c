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
#include "Geometry_Math.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define STEP_QUEUE_MAX_SIZE 5
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

typedef struct DataG2_Tag{
    Point2D_d start_point;
    Point2D_d center_point;
    Point2D_d end_point;

    double radius;
    int steps;
}DataG2;
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err step_G2(GCode_Settings* settings, GCodeCommand* cmd)
{
    Std_Err stdErr = STD_OK;
    Motor** motors = settings->motors;
    DataG2* specific_data = (DataG2*)cmd->specific_data;

    if(!(*(settings->motors_are_on)))
    {
        Point3D_d current_point3D;
        current_point3D.x = (double)motors[MOTOR_X]->data.position / ACCURACY;
        current_point3D.y = (double)motors[MOTOR_Y]->data.position / ACCURACY;
        current_point3D.z = (double)motors[MOTOR_Z]->data.position / ACCURACY;

        Point2D_d temp;
        temp = get_point2D_form_point3D(current_point3D, settings->plane_selection.plane_x,
                                                         settings->plane_selection.plane_y,
                                                         settings->plane_selection.plane_z);
        specific_data->start_point.x = temp.x;
        specific_data->start_point.y = temp.y;

        if(specific_data->steps == 0)
        {
            cmd->step = NULL;
            return STD_OK;
        }

        // TODO: parametrize is_clockwise
        // get next circle point
        Point2D_d destination = get_next_circle_point(specific_data->start_point,
            specific_data->center_point, specific_data->radius, settings->angle_step, true);

        Point3D_d destination3D = get_point3D_form_point2D(destination,
                                                           settings->plane_selection.plane_x,
                                                           settings->plane_selection.plane_y,
                                                           settings->plane_selection.plane_z);

        Point3D_d destination_err3D = {
            motors[MOTOR_X]->data.position_error / ACCURACY,
            motors[MOTOR_Y]->data.position_error / ACCURACY,
            motors[MOTOR_Z]->data.position_error / ACCURACY
        };

        destination3D.x += destination_err3D.x;
        destination3D.y += destination_err3D.y;
        destination3D.z += destination_err3D.z;

        char temp_buff[100]; //DEBUG
        uint8_t msg_size = sprintf(temp_buff, "DEST[%f %f %f]\n", destination3D.x,
            destination3D.y, destination3D.z); //DEBUG
        add_message_to_send(settings->buff_comm, temp_buff, msg_size); //DEBUG
        specific_data->steps -= 1;

        Point3D_d start_point3D = get_point3D_form_point2D(specific_data->start_point,
                                                           settings->plane_selection.plane_x,
                                                           settings->plane_selection.plane_y,
                                                           settings->plane_selection.plane_z);
        msg_size = sprintf(temp_buff, "STPT[%f %f %f]\n", start_point3D.x,
            start_point3D.y, start_point3D.z); //DEBUG
        add_message_to_send(settings->buff_comm, temp_buff, msg_size); //DEBUG
        // calculate move values
        vect3D_d move;
        move.x = destination3D.x - start_point3D.x;
        move.y = destination3D.y - start_point3D.y;
        move.z = destination3D.z - start_point3D.z;

        msg_size = sprintf(temp_buff, "MOVE[%f %f %f]\n", move.x, move.y, move.z); //DEBUG
        add_message_to_send(settings->buff_comm, temp_buff, msg_size); //DEBUG
        msg_size = sprintf(temp_buff, "POS [%f %f %f]\n",
            (double)motors[MOTOR_X]->data.position / ACCURACY,
            (double)motors[MOTOR_Y]->data.position / ACCURACY,
            (double)motors[MOTOR_Z]->data.position / ACCURACY); //DEBUG
        add_message_to_send(settings->buff_comm, temp_buff, msg_size); //DEBUG
        if(cmd->used_fields & PARAM_F)
        {
            settings->speed = cmd->data.f;
        }

        stdErr = setup_linear_movement(settings, move, settings->speed);
        if(stdErr != STD_OK) { return stdErr; }

        msg_size = sprintf(temp_buff, "STEPS[%d ]\n", specific_data->steps); //DEBUG
        add_message_to_send(settings->buff_comm, temp_buff, msg_size); //DEBUG
        stdErr = start_motors_move(settings);
    }

    return stdErr;
}


Std_Err remove_G2(GCode_Settings* settings, GCodeCommand* cmd)
{
    add_message_to_send(settings->buff_comm, "==>removeG2\n", 12);
    Std_Err stdErr = STD_OK;
    Motor** motors = settings->motors;
    DataG2* specific_data = (DataG2*)cmd->specific_data;

    // update position error
    // TODO:(should be done in time of movement calculating)
    Point3D_d end_point3D = get_point3D_form_point2D(specific_data->end_point,
                                                     settings->plane_selection.plane_x,
                                                     settings->plane_selection.plane_y,
                                                     settings->plane_selection.plane_z);

    if(settings->plane_selection.plane_x)
    {
        motors[MOTOR_X]->data.position_error = motors[MOTOR_X]->data.position -
            (int)(end_point3D.x * ACCURACY);
    }
    if(settings->plane_selection.plane_y)
    {
        motors[MOTOR_Y]->data.position_error = motors[MOTOR_Y]->data.position -
            (int)(end_point3D.y * ACCURACY);
    }
    if(settings->plane_selection.plane_z)
    {
        motors[MOTOR_Z]->data.position_error = motors[MOTOR_Z]->data.position -
            (int)(end_point3D.z * ACCURACY);
    }

    #ifdef USE_INTERRUPTS
    IRQ_DISABLE;
    #endif /* USE_INTERRUPTS */

    free(cmd->specific_data);

    #ifdef USE_INTERRUPTS
    IRQ_ENABLE;
    #endif /* USE_INTERRUPTS */

    return stdErr;
}


Std_Err init_G2(GCode_Settings* settings, GCodeCommand* cmd)
{
    Std_Err stdErr = STD_OK;
    Motor** motors = settings->motors;
    cmd->remove = remove_G2;
    cmd->step = step_G2;

    // TODO: parametrize angle step
    settings->angle_step = 10.;

    #ifdef USE_INTERRUPTS
    IRQ_DISABLE;
    #endif /* USE_INTERRUPTS */

    cmd->specific_data = malloc(sizeof(DataG2));

    #ifdef USE_INTERRUPTS
    IRQ_ENABLE;
    #endif /* USE_INTERRUPTS */

    DataG2* specific_data = (DataG2*)cmd->specific_data;

    // set helper values
    Point3D_d start_point3D, center_point3D, end_point3D;
    start_point3D.x = (double)motors[MOTOR_X]->data.position / ACCURACY;
    start_point3D.y = (double)motors[MOTOR_Y]->data.position / ACCURACY;
    start_point3D.z = (double)motors[MOTOR_Z]->data.position / ACCURACY;

    center_point3D.x = start_point3D.x + ((cmd->used_fields & PARAM_I) ? cmd->data.i : 0.);
    center_point3D.y = start_point3D.y + ((cmd->used_fields & PARAM_J) ? cmd->data.j : 0.);
    center_point3D.z = start_point3D.z + ((cmd->used_fields & PARAM_K) ? cmd->data.k : 0.);

    end_point3D.x = (cmd->used_fields & PARAM_X) ? cmd->data.x : start_point3D.x;
    end_point3D.y = (cmd->used_fields & PARAM_Y) ? cmd->data.y : start_point3D.y;
    end_point3D.z = (cmd->used_fields & PARAM_K) ? cmd->data.z : start_point3D.z;

    Point2D_d temp;
    temp = get_point2D_form_point3D(start_point3D, settings->plane_selection.plane_x,
                                                   settings->plane_selection.plane_y,
                                                   settings->plane_selection.plane_z);
    specific_data->start_point.x = temp.x;
    specific_data->start_point.y = temp.y;

    temp = get_point2D_form_point3D(center_point3D, settings->plane_selection.plane_x,
                                                    settings->plane_selection.plane_y,
                                                    settings->plane_selection.plane_z);
    specific_data->center_point.x = temp.x;
    specific_data->center_point.y = temp.y;

    temp = get_point2D_form_point3D(end_point3D, settings->plane_selection.plane_x,
                                                    settings->plane_selection.plane_y,
                                                    settings->plane_selection.plane_z);
    specific_data->end_point.x = temp.x;
    specific_data->end_point.y = temp.y;


    // check length of radius
    specific_data->radius = get_distance_between_points(specific_data->start_point,
        specific_data->center_point);
    double end_radius = get_distance_between_points(specific_data->end_point,
        specific_data->center_point);

    if(!compare_doubles(specific_data->radius, end_radius, 1./ACCURACY))
    {
        #ifdef USE_INTERRUPTS
        IRQ_DISABLE;
        #endif /* USE_INTERRUPTS */

        free(cmd->specific_data);

        #ifdef USE_INTERRUPTS
        IRQ_ENABLE;
        #endif /* USE_INTERRUPTS */

        return STD_PARAMETER_ERROR;
    }

    // calculate number of moves
    vect2D_d start_vect = getVector2D(specific_data->center_point.x, specific_data->center_point.y,
                                      specific_data->start_point.x,  specific_data->start_point.y);
    vect2D_d end_vect   = getVector2D(specific_data->center_point.x, specific_data->center_point.y,
                                      specific_data->end_point.x,    specific_data->end_point.y);
    double angle = get_angle_in_degrees(getAngleBetweenVectors2D(start_vect, end_vect));
    char temp_buff[100]; //DEBUG
    uint8_t msg_size = sprintf(temp_buff, "ANGLE1[%f]\n", angle); //DEBUG
    add_message_to_send(settings->buff_comm, temp_buff, msg_size); //DEBUG
    angle = (angle == 0) ? 360 : angle;
    msg_size = sprintf(temp_buff, "ANGLE2[%f]\n", angle); //DEBUG
    add_message_to_send(settings->buff_comm, temp_buff, msg_size); //DEBUG

    specific_data->steps = round(angle / settings->angle_step);

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

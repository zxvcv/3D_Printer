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
        // update current position
        if(settings->plane_selection.plane_x)
        {
            specific_data->start_point.x = motors[MOTOR_X]->data.position / ACCURACY;

            if(settings->plane_selection.plane_y)
            {
                specific_data->start_point.y = motors[MOTOR_Y]->data.position / ACCURACY;
            }
        }

        if(settings->plane_selection.plane_z)
        {
            if(settings->plane_selection.plane_y)
            {
                specific_data->start_point.x =  motors[MOTOR_Y]->data.position / ACCURACY;
            }

            specific_data->start_point.y = motors[MOTOR_Z]->data.position / ACCURACY;
        }

        if(specific_data->steps == 0)
        {
            cmd->step = NULL;
            return STD_OK;
        }

        // TODO: parametrize angle step and is_clockwise
        // get next circle point
        Point2D_d destination = get_next_circle_point(specific_data->start_point,
            specific_data->center_point, specific_data->radius, 1., true);

        specific_data->steps -= 1;

        // calculate move values
        vect3D_d move;
        move.x = (cmd->used_fields & PARAM_X) ?
            (destination.x - specific_data->start_point.x) : 0.;
        if(cmd->used_fields & PARAM_Y)
        {
            move.y = (cmd->used_fields & PARAM_X) ?
                destination.y - specific_data->start_point.y :
                destination.x - specific_data->start_point.x;
        }
        else
        {
            move.y = 0.;
        }
        move.z = cmd->used_fields & PARAM_Z ?
            destination.y - specific_data->start_point.y : 0.;

        if(cmd->used_fields & PARAM_F)
        {
            settings->speed = cmd->data.f;
        }

        stdErr = setup_linear_movement(settings, move, settings->speed);
        if(stdErr != STD_OK) { return stdErr; }

        stdErr = start_motors_move(settings);
    }

    return stdErr;
}


Std_Err remove_G2(GCode_Settings* settings, GCodeCommand* cmd)
{
    Std_Err stdErr = STD_OK;

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
    cmd->remove = NULL;
    cmd->step = step_G2;


    #ifdef USE_INTERRUPTS
    IRQ_DISABLE;
    #endif /* USE_INTERRUPTS */

    cmd->specific_data = malloc(sizeof(DataG2));

    #ifdef USE_INTERRUPTS
    IRQ_ENABLE;
    #endif /* USE_INTERRUPTS */

    DataG2* specific_data = (DataG2*)cmd->specific_data;

    // set helper values
    if(settings->plane_selection.plane_x)
    {
        specific_data->start_point.x = motors[MOTOR_X]->data.position / ACCURACY;
        specific_data->center_point.x = specific_data->start_point.x +
            ((cmd->used_fields & PARAM_I) ? cmd->data.i : 0.);
        specific_data->end_point.x =
            (cmd->used_fields & PARAM_X) ? cmd->data.x : specific_data->start_point.x;

        if(settings->plane_selection.plane_y)
        {
            specific_data->start_point.y = motors[MOTOR_Y]->data.position / ACCURACY;
            specific_data->center_point.y = specific_data->start_point.y +
                ((cmd->used_fields & PARAM_J) ? cmd->data.j : 0.);
            specific_data->end_point.y =
                (cmd->used_fields & PARAM_Y) ? cmd->data.y : specific_data->start_point.y;
        }
    }

    if(settings->plane_selection.plane_z)
    {
        if(settings->plane_selection.plane_y)
        {
            specific_data->start_point.x =  motors[MOTOR_Y]->data.position / ACCURACY;
            specific_data->center_point.x = specific_data->start_point.x +
                ((cmd->used_fields & PARAM_J) ? cmd->data.j : 0.);
            specific_data->end_point.x =
                (cmd->used_fields & PARAM_Y) ? cmd->data.y : specific_data->start_point.x;
        }

        specific_data->start_point.y = motors[MOTOR_Z]->data.position / ACCURACY;
        specific_data->center_point.y = specific_data->start_point.y +
            ((cmd->used_fields & PARAM_K) ? cmd->data.k : 0.);
        specific_data->end_point.y =
            (cmd->used_fields & PARAM_Z) ? cmd->data.z : specific_data->start_point.y;
    }

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
    double angle = getAngleBetweenVectors2D(start_vect, end_vect);
    angle = (angle == 0) ? 360 : angle;

    // TODO: parametrize angle step
    specific_data->steps = round(angle / 1.);

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

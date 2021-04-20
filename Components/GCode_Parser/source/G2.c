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
#include "_helpers.h"
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
    Fifo_C* step_queue;

    point3D_d start_point;
    point3D_d center_point;
    point3D_d end_point;
    point3D_i step_size;

    double radius;
    bool full_circle;
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

    uint8_t fifo_size = fifo_getSize(specific_data->step_queue);

    if(!(*(settings->motors_are_on)))
    {
        // update current position
        if(settings->plane_selection.plane_x)
        {
            specific_data->start_point.a = motors[MOTOR_X]->data.position / ACCURACY;

            if(settings->plane_selection.plane_y)
            {
                specific_data->start_point.b = motors[MOTOR_Y]->data.position / ACCURACY;
            }
        }

        if(settings->plane_selection.plane_z)
        {
            if(settings->plane_selection.plane_y)
            {
                specific_data->start_point.a =  motors[MOTOR_Y]->data.position / ACCURACY;
            }

            specific_data->start_point.b = motors[MOTOR_Z]->data.position / ACCURACY;
        }

        if(compare_points(specific_data->start_point, specific_data->end_point, 1./ACCURACY))
        {
            cmd->step = NULL;
            return STD_OK;
        }

        if(fifo_size > 0)
        {
            // get data to set the move
            point3D_d* destination;

            stdErr = fifo_front(specific_data->step_queue, (void**)(&destination));
            if(stdErr != STD_OK) { return stdErr; }

            // calculate move values
            if(cmd->used_fields & PARAM_F)
            {
                settings->speed = cmd->data.f;
            }

            vect3D_d move;
            move.x = (cmd->used_fields & PARAM_X) ?
                (destination->a - specific_data->start_point.a) : 0.;
            if(cmd->used_fields & PARAM_Y)
            {
                move.y = (cmd->used_fields & PARAM_X) ?
                    destination->b - specific_data->start_point.b :
                    destination->a - specific_data->start_point.a;
            }
            else
            {
                move.y = 0.;
            }
            move.z = cmd->used_fields & PARAM_Z ?
                destination->b - specific_data->start_point.b : 0.;

            vect3D_d velocity = getVelocity3D(move, settings->speed);

            MotorCounters counters_val;
            bool direction;

            double move_tab[MOTORS_NUM] = { move.x, move.y, move.z , 0. };
            double velocity_tab[MOTORS_NUM] =
                { fabs(velocity.x), fabs(velocity.y), fabs(velocity.z), fabs(0.) };

            for(int i=MOTOR_X; i<MOTORS_NUM; ++i)
            {
                memset(&counters_val, 0, sizeof(MotorCounters));

                stdErr = motor_get_linear_move_settings(motors[i],
                                move_tab[i],
                                velocity_tab[i],
                                ACCURACY, &counters_val, &direction);
                if(stdErr != STD_OK) { return stdErr; }

                motor_set_counters(motors[i], &counters_val);
                motor_set_direction(motors[i], direction);
            }

            stdErr = fifo_pop_C(specific_data->step_queue);
            if(stdErr != STD_OK) { return stdErr; }

            for(int i=MOTOR_X; i<MOTORS_NUM; ++i)
            {
                stdErr = motor_start(motors[i]);
                if(stdErr != STD_OK) { return stdErr; }
            }
            *(settings->motors_are_on) = true;
        }
    }

    if(fifo_size < STEP_QUEUE_MAX_SIZE &&
      (specific_data->full_circle ||
       !compare_points(specific_data->start_point, specific_data->end_point, 1./ACCURACY)))
    {

        // get next circle line
        point3D_d destination = get_next_circle_line(specific_data->start_point,
            specific_data->end_point, specific_data->center_point, specific_data->radius,
            specific_data->step_size, true);

        // add next step point to queue
        stdErr = fifo_push_C(specific_data->step_queue, (void*)&destination, sizeof(point3D_d));
        if(stdErr != STD_OK) { return stdErr; }
    }

    return stdErr;
}


Std_Err remove_G2(GCode_Settings* settings, GCodeCommand* cmd)
{
    Std_Err stdErr = STD_OK;
    DataG2* specific_data = (DataG2*)cmd->specific_data;

    stdErr = fifo_delete_C(&(specific_data->step_queue));
    if(stdErr != STD_OK) { return stdErr; }

    #ifdef USE_INTERRUPTS
    IRQ_DISABLE;
    #endif /* USE_INTERRUPTS */

    free(cmd->specific_data);

    #ifdef USE_INTERRUPTS
    IRQ_ENABLE;
    #endif /* USE_INTERRUPTS */

    return STD_OK;
}


Std_Err init_G2(GCode_Settings* settings, GCodeCommand* cmd)
{
    Std_Err stdErr = STD_OK;
    Motor** motors = settings->motors;
    cmd->remove = NULL;
    cmd->step = step_G2;

    uint8_t cnt = 0;
    for(int i=PARAM_I; i<=PARAM_K; i<<=1)
    {
        if(cmd->used_fields & i)
        {
            cnt += 1;
        }
    }

    #ifdef USE_INTERRUPTS
    IRQ_DISABLE;
    #endif /* USE_INTERRUPTS */

    cmd->specific_data = malloc(sizeof(DataG2));

    #ifdef USE_INTERRUPTS
    IRQ_ENABLE;
    #endif /* USE_INTERRUPTS */

    DataG2* specific_data = (DataG2*)cmd->specific_data;

    // set helper values
    specific_data->full_circle = cnt == 0 ? true : false;
    if(cnt != 0 || cnt != 2)
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

    if(settings->plane_selection.plane_x)
    {
        specific_data->start_point.a = motors[MOTOR_X]->data.position / ACCURACY;
        specific_data->center_point.a = specific_data->start_point.a +
            (cmd->used_fields & PARAM_I) ? cmd->data.i : 0.;
        specific_data->end_point.a =
            (cmd->used_fields & PARAM_X) ? cmd->data.x : specific_data->start_point.a;
        specific_data->step_size.a = motors[MOTOR_X]->settings.step_size;

        if(settings->plane_selection.plane_y)
        {
            specific_data->start_point.b = motors[MOTOR_Y]->data.position / ACCURACY;
            specific_data->center_point.b = specific_data->start_point.b +
                (cmd->used_fields & PARAM_J) ? cmd->data.j : 0.;
            specific_data->end_point.b =
                (cmd->used_fields & PARAM_Y) ? cmd->data.y : specific_data->start_point.b;
            specific_data->step_size.b = motors[MOTOR_Y]->settings.step_size;
        }
    }

    if(settings->plane_selection.plane_z)
    {
        if(settings->plane_selection.plane_y)
        {
            specific_data->start_point.a =  motors[MOTOR_Y]->data.position / ACCURACY;
            specific_data->center_point.a = specific_data->start_point.a +
                (cmd->used_fields & PARAM_J) ? cmd->data.j : 0.;
            specific_data->end_point.a =
                (cmd->used_fields & PARAM_Y) ? cmd->data.y : specific_data->start_point.a;
            specific_data->step_size.a = motors[MOTOR_Y]->settings.step_size;
        }

        specific_data->start_point.b = motors[MOTOR_Z]->data.position / ACCURACY;
        specific_data->center_point.b = specific_data->start_point.b +
            (cmd->used_fields & PARAM_K) ? cmd->data.k : 0.;
        specific_data->end_point.b =
            (cmd->used_fields & PARAM_Z) ? cmd->data.z : specific_data->start_point.b;
        specific_data->step_size.b = motors[MOTOR_Z]->settings.step_size;
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

    // fifo init
    stdErr = fifo_create(&(specific_data->step_queue));

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

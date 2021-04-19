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

    double radius;
    bool full_circle;
}DataG2;
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err step_G2(GCode_Settings* settings, GCodeCommand* cmd)
{
    DataG2* specific_data = (DataG2*)cmd->specific_data;

    uint8_t fifo_size = fifo_getSize(specific_data->step_queue);

    if(!(*(settings->motors_are_on)))
    {
        if(compare_points(/*current_point*/, specific_data->end_point, 1./ACCURACY)
        {
            cmd->step = NULL;
            return STD_OK;
        }

        if(fifo_size > 0)
        {
            // get data to set the move
            // begin move
        }
    }

    if(fifo_size < STEP_QUEUE_MAX_SIZE && /*move data not ends yet*/)
    {
        // get next circle line
        // calculate move values
        // add calculated move values to fifo
    }
}


Std_Err remove_G2(GCode_Settings* settings, GCodeCommand* cmd)
{
    DataG2* specific_data = (DataG2*)cmd->specific_data;

    stdErr = fifo_delete_C(&(specific_data->step_queue))
    if(stdErr != STD_OK) { return stdErr; }

    #ifdef USE_INTERRUPTS
    IRQ_DISABLE
    #endif /* USE_INTERRUPTS */

    free(cmd->specific_data);

    #ifdef USE_INTERRUPTS
    IRQ_ENABLE
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
        if(settings->used_fields & i)
        {
            cnt += 1;
        }
    }

    #ifdef USE_INTERRUPTS
    IRQ_DISABLE
    #endif /* USE_INTERRUPTS */

    cmd->specific_data = malloc(sizeof(DataG2));

    #ifdef USE_INTERRUPTS
    IRQ_ENABLE
    #endif /* USE_INTERRUPTS */

    DataG2* specific_data = (DataG2*)cmd->specific_data;

    // set helper values
    specific_data->full_circle = cnt == 0 ? true : false;
    if(cnt != 0 || cnt != 2)
    {
        #ifdef USE_INTERRUPTS
        IRQ_DISABLE
        #endif /* USE_INTERRUPTS */

        free(cmd->specific_data);

        #ifdef USE_INTERRUPTS
        IRQ_ENABLE
        #endif /* USE_INTERRUPTS */

        return STD_PARAMETER_ERROR;
    }

    if(settings->plane_selection.plane_x)
    {
        specific_data->start_point.a =
            (motors[MOTOR_X]->data.position + motors[MOTOR_X]->data.position_error) / ACCURACY;
        specific_data->center_point.a = specific_data->start_point.a +
            (settings->used_fields & PARAM_I) ? settings->data.i : 0.;
        specific_data->end_point.a =
            (settings->used_fields & PARAM_X) ? settings->data.x : specific_data->start_point.a;

        if(settings->plane_selection.plane_y)
        {
            specific_data->start_point.b =
                (motors[MOTOR_Y]->data.position + motors[MOTOR_Y]->data.position_error) / ACCURACY;
            specific_data->center_point.b = specific_data->start_point.b +
                (settings->used_fields & PARAM_J) ? settings->data.j : 0.;
            specific_data->end_point.b =
                (settings->used_fields & PARAM_Y) ? settings->data.y : specific_data->start_point.b;
        }
    }

    if(settings->plane_selection.plane_z)
    {
        if(settings->plane_selection.plane_y)
        {
            specific_data->start_point.a =
                (motors[MOTOR_Y]->data.position + motors[MOTOR_Y]->data.position_error) / ACCURACY;
            specific_data->center_point.a = specific_data->start_point.a +
                (settings->used_fields & PARAM_J) ? settings->data.j : 0.;
            specific_data->end_point.a =
                (settings->used_fields & PARAM_Y) ? settings->data.y : specific_data->start_point.a;
        }

        specific_data->start_point.b =
            (motors[MOTOR_Z]->data.position + motors[MOTOR_Z]->data.position_error) / ACCURACY;
        specific_data->center_point.b = specific_data->start_point.b +
            (settings->used_fields & PARAM_K) ? settings->data.k : 0.;
        specific_data->end_point.b =
            (settings->used_fields & PARAM_Z) ? settings->data.z : specific_data->start_point.b;
    }

    // check length of radius
    specific_data->radius = get_distance_between_points(specific_data->start_point,
        specific_data->center_point);
    double end_radius = get_distance_between_points(specific_data->end_point,
        specific_data->center_point);
    if(!compare_points(specific_data->radius, end_radius, 1./ACCURACY))
    {
        #ifdef USE_INTERRUPTS
        IRQ_DISABLE
        #endif /* USE_INTERRUPTS */

        free(cmd->specific_data);

        #ifdef USE_INTERRUPTS
        IRQ_ENABLE
        #endif /* USE_INTERRUPTS */

        return STD_PARAMETER_ERROR;
    }

    // fifo init
    stdErr = fifo_create(&(specific_data->step_queue));

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

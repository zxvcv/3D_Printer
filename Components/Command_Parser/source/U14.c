/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Command_Parser
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

typedef struct Data_U14_Tag{
    bool detection_is_progress;

    bool axisX_min;
    bool axisX_max;

    bool axisY_min;
    bool axisY_max;

    bool axisZ_min;
    bool axisZ_max;
}Data_U14;
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

Std_Err onDetection_max(BoundDetector* settings, void* event_data)
{
    Data_U14* specific_data = (Data_U14*)event_data;

    // TODO: set position end as current position
    // TODO: set current motor position as 0

    reset_onDetection_event(settings);
    specific_data->detection_is_progress = false;

    return STD_OK;
}

Std_Err onDetection_min(BoundDetector* settings, void* event_data)
{
    Data_U14* specific_data = (Data_U14*)event_data;

    // TODO: set position zero as 0
    // TODO: set current motor position as 0

    reset_onDetection_event(settings);
    specific_data->detection_is_progress = false;

    return STD_OK;
}

Std_Err step_U14(SystemCommand_Settings* settings, SystemCommand* cmd)
{
    Std_Err stdErr = STD_OK;
    Data_U14* specific_data = (Data_U14*)cmd->specific_data;
    Motor** motors = settings->motors;

    if(!(specific_data->detection_is_progress))
    {
        if(specific_data->axisX_min)
        {
            specific_data->detection_is_progress = true;
            set_onDetection_event(&(settings->boundaryDetection->bound_MinX), onDetection_min,
                (void*)specific_data);

            motor_set_position_zero(motors[MOTOR_X], -1000);
            motor_set_position(motors[MOTOR_X], 0);
            stdErr = forward_command_concurrently(settings, cmd, "G1 X-1000", false);
        }
        else if(specific_data->axisX_max)
        {
            specific_data->detection_is_progress = true;
            set_onDetection_event(&(settings->boundaryDetection->bound_MaxX), onDetection_min,
                (void*)specific_data);

            motor_set_position_end(motors[MOTOR_X], 1000);
            motor_set_position(motors[MOTOR_X], 0);
            stdErr = forward_command_concurrently(settings, cmd, "G1 X1000", false);
        }

        // TODO: after set min and max for axis move this axis to middle position

        // TODO: expand functionality to Y and Z axis
    }
    else
    {
        if(cmd->gcode_cmd.remove == NULL)
        {
            specific_data->detection_is_progress = false;

            if(specific_data->axisX_min)
            {
                motor_set_position_zero(motors[MOTOR_X], 0);
                motor_set_position(motors[MOTOR_X], 0);
                specific_data->axisX_min = false;
            }
            else if(specific_data->axisX_max)
            {
                motor_set_position_end(motors[MOTOR_X], motors[MOTOR_X]->data.position);
                specific_data->axisX_max = false;
            }
        }
    }

    return stdErr;
}


Std_Err remove_U14(SystemCommand_Settings* settings, SystemCommand* cmd)
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

Std_Err init_U14(SystemCommand_Settings* settings, SystemCommand* cmd)
{
    Std_Err stdErr = STD_OK;
    Data_U14* specific_data = NULL;

    cmd->remove = remove_U14;
    cmd->step = step_U14;

    #ifdef USE_INTERRUPTS
    IRQ_DISABLE;
    #endif /* USE_INTERRUPTS */

    cmd->specific_data = malloc(sizeof(Data_U14));

    #ifdef USE_INTERRUPTS
    IRQ_ENABLE;
    #endif /* USE_INTERRUPTS */

    specific_data = (Data_U14*)cmd->specific_data;

    specific_data->axisX_min = specific_data->axisX_max =
        (cmd->used_fields & PARAM_X) ? true : false;
    specific_data->axisY_min = specific_data->axisY_max =
        (cmd->used_fields & PARAM_Y) ? true : false;
    specific_data->axisZ_min = specific_data->axisZ_max =
        (cmd->used_fields & PARAM_Z) ? true : false;
    specific_data->detection_is_progress = false;

    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

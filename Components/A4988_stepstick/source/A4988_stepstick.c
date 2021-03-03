/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: A4988_stepstick
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "A4988_stepstick.h"
#include <math.h>
#include <stdlib.h>
#include <limits.h>
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define ACCURACY 1000

#define HIGH    1
#define LOW     0
#define CLOCKWISE           1
#define COUNTER_CLOCKWISE   0
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err motor_update_pins(Motor* motor)
{
    if(motor->IOreset.PORT != NULL)
    {
        HAL_GPIO_WritePin(motor->IOreset.PORT, motor->IOreset.PIN,
                ((motor->flags.reset) ? GPIO_PIN_RESET : GPIO_PIN_SET));
    }

    if(motor->IOsleep.PORT != NULL)
    {
        HAL_GPIO_WritePin(motor->IOsleep.PORT, motor->IOsleep.PIN,
                ((motor->flags.sleep) ? GPIO_PIN_RESET : GPIO_PIN_SET));
    }

    if(motor->IOdirection.PORT != NULL)
    {
        // [TODO]: UT on this
        bool direction = motor->flags.direction ^ motor->flags.reversed;
        HAL_GPIO_WritePin(motor->IOdirection.PORT, motor->IOdirection.PIN,
                ((direction) ? GPIO_PIN_SET : GPIO_PIN_RESET));
    }

    if(motor->IOstep.PORT != NULL)
    {
        HAL_GPIO_WritePin(motor->IOstep.PORT, motor->IOstep.PIN,
                ((motor->flags.stepPhase) ? GPIO_PIN_SET : GPIO_PIN_RESET));
    }
}


Std_Err motor_init(Motor* motor)
{
    motor->flags.isOn = 0;
    motor->flags.reset = 0;
    motor->flags.sleep = 1;
    motor->flags.stepPhase = LOW;
    motor->flags.direction = CLOCKWISE;
    motor->flags.reversed = 0;

    motor->counters.timer = 0;
    motor->counters.timer_start = 0;
    motor->counters.repeat = 0;

    motor->settings.timer_frequency = 0.0;
    motor->settings.step_size = 0;
    motor->settings.max_speed = 0;
    motor->settings.position_zero = 0;
    motor->settings.position_end = 0;

    motor_update_pins(motor);
}


Std_Err motor_update(Motor* motor)
{
    Std_Err retVal = STD_OK;

    if(motor->flags.isOn && !motor->flags.reset && !motor->flags.sleep)
    {
        --(motor->counters.timer);
        if(motor->counters.timer <= 0)
        {
            motor->counters.timer = motor->counters.timer_start;
            --(motor->counters.steps);
            if(motor->counters.steps <= 0)
            {
                motor->flags.isOn = 0;
                motor->flags.sleep = 1;
            }

            switch (motor->flags.stepPhase)
            {
                case HIGH:
                    motor->flags.stepPhase = LOW;
                    break;
                case LOW:
                    motor->flags.stepPhase = HIGH;
                    if(motor->flags.direction == CLOCKWISE)
                        motor->data.position -= motor->device.step_size;
                    else
                        motor->data.position += motor->device.step_size;
                    break;
                default:
                    retVal = STD_REFERENCE_ERROR;
                    break;
            }

            if(retVal == STD_OK)
            {
                motor_update_pins(motor);
            }
        }
    }
    else
    {
        retVal = STD_ERROR;
    }

    return retVal;
}


Std_Err motor_set_counters(Motor* motor, uint16_t timer, uint16_t timer_start, uint8_t steps)
{
    motor->counters.timer = timer;
    motor->counters.timer_start = timer_start;
    motor->counters.steps = steps;

    return STD_OK;
}


Std_Err motor_start(Motor* motor)
{
    if(motor->counters.timer <= 0 || motor->counters.timer_start <= 0 || motor->counters.steps <= 0)
    {
        return STD_PARAMETER_ERROR
    }

    if(motor->flags.reset)
    {
        return STD_ERROR;
    }

    if(motor->flags.stepPhase != LOW)
    {
        return STD_ERROR;
    }

    settings->flags.sleep = 0;
    settings->flags.isOn = 1;

    motor_update_pins(motor);

    return STD_OK;
}


Std_Err motor_stop(Motor* motor)
{
    Std_Err retVal = STD_OK;

    if(motor->flags.reset)
    {
        return STD_ERROR;
    }

    motor->flags.isOn = 0;
    motor->flags.sleep = 1;

    if(motor->flags.stepPhase != LOW)
    {
        motor->flags.stepPhase = LOW;
        retVal = STD_IO_ERROR;
    }

    motor_update_pins(motor);

    if(motor->counters.steps > 0 || motor->counters.timer > 0)
    {
        retVal = STD_INTERRUPTED_ERROR;
    }

    return retVal;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

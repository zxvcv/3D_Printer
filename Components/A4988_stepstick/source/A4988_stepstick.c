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
#include "stm32f3xx_hal.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define HIGH    1
#define LOW     0
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void motor_update_pins(Motor* motor)
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


void motor_init(Motor* motor, double timer_frequency,
                IO_Pin reset_pins, IO_Pin sleep_pins, IO_Pin direction_pins, IO_Pin step_pins)
{
    motor->IOreset.PORT = reset_pins.PORT;
    motor->IOreset.PIN = reset_pins.PIN;
    motor->IOsleep.PORT = sleep_pins.PORT;
    motor->IOsleep.PIN = sleep_pins.PIN;
    motor->IOdirection.PORT = direction_pins.PORT;
    motor->IOdirection.PIN = direction_pins.PIN;
    motor->IOstep.PORT = step_pins.PORT;
    motor->IOstep.PIN = step_pins.PIN;

    motor->flags.isOn = 0;
    motor->flags.reset = 0;
    motor->flags.sleep = 1;
    motor->flags.stepPhase = LOW;
    motor->flags.direction = CLOCKWISE;
    motor->flags.reversed = 0;

    motor->counters.timer = 0;
    motor->counters.timer_start = 0;
    motor->counters.steps = 0;

    motor->settings.timer_frequency = timer_frequency;
    motor->settings.step_size = 0;
    motor->settings.max_speed = 0;
    motor->settings.position_zero = 0;
    motor->settings.position_end = 0;

    motor_update_pins(motor);
}


Std_Err motor_start(Motor* motor)
{
    if(motor->counters.timer <= 0 || motor->counters.timer_start <= 0 || motor->counters.steps <= 0)
    {
        return STD_PARAMETER_ERROR;
    }

    if(motor->flags.reset)
    {
        return STD_ERROR;
    }

    if(motor->flags.stepPhase != LOW)
    {
        return STD_ERROR;
    }

    motor->flags.sleep = 0;
    motor->flags.isOn = 1;

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
        // retVal = STD_IO_ERROR;
    }

    motor_update_pins(motor);

    if(motor->counters.steps > 0)
    {
        retVal = STD_INTERRUPTED_ERROR;
    }

    return retVal;
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
                        motor->data.position -= motor->settings.step_size;
                    else
                        motor->data.position += motor->settings.step_size;
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


void motor_set_counters(Motor* motor, MotorCounters* counters)
{
    motor->counters = *counters;
}


void motor_set_direction(Motor* motor, unsigned int direction)
{
    if(direction == CLOCKWISE)
    {
        motor->flags.direction = CLOCKWISE;
    }
    else if(direction == COUNTER_CLOCKWISE)
    {
        motor->flags.direction = COUNTER_CLOCKWISE;
    }
}


Std_Err motor_set_position(Motor* motor, double position)
{
    int new_position = ((int)(position * ACCURACY)) / motor->settings.step_size;
    int new_position_error = ((int)(position * ACCURACY)) % motor->settings.step_size;

    if(new_position < motor->settings.position_zero || new_position > motor->settings.position_end)
    {
        return STD_PARAMETER_ERROR;
    }

    motor->data.position = new_position;
    motor->data.position_error = new_position_error;

    return STD_OK;
}


Std_Err motor_get_linear_move_settings(Motor* motor, double move, double speed, const int accuracy,
                                       MotorCounters* counters, bool* direction)
{
    const int _move_int = (int)round(move * accuracy);
    const int _position = motor->data.position;
    const int _position_zero = motor->settings.position_zero;
    const int _position_end = motor->settings.position_end;

    const int _move_sign = (signbit(move) ? -1 : 1);

    const int _abs_move = abs(_move_int);
    const int _step_size = motor->settings.step_size;

    /* speed */
    if((speed <= 0 || speed > motor->settings.max_speed) && (_abs_move > _step_size / 2))
    {
        return STD_PARAMETER_ERROR;
    }

    double change_freq = speed / ((double)_step_size / accuracy);
    double change_timeD = motor->settings.timer_frequency / (change_freq * 2);

    counters->timer = (uint16_t)change_timeD;
    counters->timer_start = (uint16_t)change_timeD;

    /* move */
    int steps_num1 = _abs_move / _step_size;
    int steps_num2 = steps_num1 + 1;
    int accuracy1 = (steps_num1 * _step_size) - _abs_move;
    int accuracy2 = (steps_num2 * _step_size) - _abs_move;

    int new_position1 = (steps_num1 * _step_size * _move_sign) + _position;
    int new_position2 = (steps_num2 * _step_size * _move_sign) + _position;

    if(new_position1 > _position_end || new_position1 < _position_zero)
        accuracy1 = INT_MAX;
    if(new_position2 > _position_end || new_position2 < _position_zero)
        accuracy2 = INT_MAX;

    if(accuracy1 == INT_MAX && accuracy2 == INT_MAX)
    {
        counters->timer = 0;
        counters->timer_start = 0;

        return STD_PARAMETER_ERROR;
    }
    else if(abs(accuracy1) <= abs(accuracy2))
    {
        counters->steps = steps_num1 * 2;
        motor->data.position_error = abs(accuracy1) * _move_sign;
    }
    else
    {
        counters->steps = steps_num2 * 2;
        motor->data.position_error = abs(accuracy2) * _move_sign * ((int)-1);
    }

    // /*TODO: check calculating speedError*/
    // roundingError->speedError = speed -
    //     (((settings->device.timer_frequency * _step_size) / accuracy) / (settings->change_time * 2));

    /* direction */
    if(_move_sign > 0)
        *direction = COUNTER_CLOCKWISE;
    else
        *direction = CLOCKWISE;

    return STD_OK;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

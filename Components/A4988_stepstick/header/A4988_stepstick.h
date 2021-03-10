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
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      None
 ************************************************************************************************/

#ifndef A4988_STEPSTICK_H_
#define A4988_STEPSTICK_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include <stdbool.h>
#include <stdint.h>
#include "IOpin.h"
#include "Error_Codes.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define CLOCKWISE           1
#define COUNTER_CLOCKWISE   0
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct MotorCounters_Tag{
    uint16_t timer;
    uint16_t timer_start;
    uint8_t steps;
}MotorCounters;

typedef struct Motor_Tag{
    IO_Pin IOreset;
    IO_Pin IOsleep;
    IO_Pin IOdirection;
    IO_Pin IOstep;

    struct{
        unsigned int isOn       :1;     /*(1-on         0-off               )*/
        unsigned int reset      :1;     /*(1-yes        0-no                )*/
        unsigned int sleep      :1;     /*(1-yes        0-no                )*/
        unsigned int stepPhase  :1;     /*(1-high       0-low               )*/
        unsigned int direction  :1;     /*(1-clockwise, 0-counter clockwise )*/
        unsigned int reversed   :1;     /*(1-yes        0-no                )*/
    }flags;

    MotorCounters counters;

    struct{
        double timer_frequency;  //[Hz] timer frequency
        int step_size;           //[mm/ACCURACY] length of move with one motor step = (stepSize * ACCURACY)
        double max_speed;
        int position_zero;
        int position_end;
    }settings;

    struct{
        int position;
        int position_error;
    }data;
} Motor;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void motor_update_pins(Motor* motor);

void motor_init(Motor* motor);

Std_Err motor_update(Motor* motor);

Std_Err motor_start(Motor* motor);

Std_Err motor_stop(Motor* motor);

void motor_set_counters(Motor* motor, MotorCounters* counters);

void motor_set_direction(Motor* motor, unsigned int direction);

Std_Err motor_set_position(Motor* motor, double position);

Std_Err motor_get_linear_move_settings(Motor* motor, double move, double speed, const int ACCURACY,
                                       MotorCounters* counters, bool* direction);

/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* A4988_STEPSTICK_H_ */

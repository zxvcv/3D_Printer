#include "Mock_A4988_stepstick.h"


Mock_A4988_stepstick* Mock_A4988_stepstick::mock;

extern "C"
{
    void motor_update_pins(Motor* motor)
    {
        Mock_A4988_stepstick::mock->motor_update_pins(motor);
    }

    void motor_init(Motor* motor, double timer_frequency, IO_Pin reset_pins, IO_Pin sleep_pins,
        IO_Pin direction_pins, IO_Pin step_pins)
    {
        Mock_A4988_stepstick::mock->motor_init(motor, timer_frequency, reset_pins, sleep_pins,
            direction_pins, step_pins);
    }

    Std_Err motor_update(Motor* motor)
    {
        return Mock_A4988_stepstick::mock->motor_update(motor);
    }

    Std_Err motor_start(Motor* motor)
    {
        return Mock_A4988_stepstick::mock->motor_start(motor);
    }

    Std_Err motor_stop(Motor* motor)
    {
        return Mock_A4988_stepstick::mock->motor_stop(motor);
    }

    void motor_set_counters(Motor* motor, MotorCounters* counters)
    {
        Mock_A4988_stepstick::mock->motor_set_counters(motor, counters);
    }

    void motor_set_direction(Motor* motor, unsigned int direction)
    {
        Mock_A4988_stepstick::mock->motor_set_direction(motor, direction);
    }

    Std_Err motor_set_position(Motor* motor, double position)
    {
        return Mock_A4988_stepstick::mock->motor_set_position(motor, position);
    }

    Std_Err motor_get_linear_move_settings(Motor* motor, double move, double speed, const int accuracy,
                                           MotorCounters* counters, bool* direction)
    {
        return Mock_A4988_stepstick::mock->motor_get_linear_move_settings(motor, move, speed, accuracy,
                                                                   counters, direction);
    }
}


#include "Mock_A4988_stepstick.h"


Mock_A4988_stepstick* Mock_A4988_stepstick::mock;

extern "C"
{
    void motor_update_pins(Motor* motor)
    {
        Mock_A4988_stepstick::mock->motor_update_pins(motor);
    }

    void motor_init(Motor* motor)
    {
        Mock_A4988_stepstick::mock->motor_init(motor);
    }

    Std_Err motor_update(Motor* motor)
    {
        Mock_A4988_stepstick::mock->motor_update(motor);
    }

    Std_Err motor_start(Motor* motor)
    {
        Mock_A4988_stepstick::mock->motor_start(motor);
    }

    Std_Err motor_stop(Motor* motor)
    {
        Mock_A4988_stepstick::mock->motor_stop(motor);
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
        Mock_A4988_stepstick::mock->motor_set_position(motor, position);
    }

    Std_Err motor_get_linear_move_settings(Motor* motor, double move, double speed, const int ACCURACY,
                                           MotorCounters* counters, bool* direction);
    {
        Mock_A4988_stepstick::mock->motor_get_linear_move_settings(motor, move, speed, ACCURACY,
                                                                   counters, direction);
    }
}


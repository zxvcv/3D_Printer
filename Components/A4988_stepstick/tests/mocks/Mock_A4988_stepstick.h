#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "A4988_stepstick.h"
}

class Mock_A4988_stepstick {
public:
    Mock_A4988_stepstick() { mock = this; }
    MOCK_METHOD1(motor_update_pins, void(Motor* motor));
    MOCK_METHOD6(motor_init, void(Motor* motor, double timer_frequency, IO_Pin reset_pins,
        IO_Pin sleep_pins, IO_Pin direction_pins, IO_Pin step_pins));
    MOCK_METHOD1(motor_update, Std_Err(Motor* motor));
    MOCK_METHOD1(motor_start, Std_Err(Motor* motor));
    MOCK_METHOD1(motor_stop, Std_Err(Motor* motor));
    MOCK_METHOD2(motor_set_counters, void(Motor* motor, MotorCounters* counters));
    MOCK_METHOD2(motor_set_direction, void(Motor* motor, unsigned int direction));
    MOCK_METHOD2(motor_set_position, Std_Err(Motor* motor, double position));
    MOCK_METHOD6(motor_get_linear_move_settings, Std_Err(Motor* motor, double move, double speed,
        const int accuracy, MotorCounters* counters, bool* direction));


    static Mock_A4988_stepstick* mock;
};

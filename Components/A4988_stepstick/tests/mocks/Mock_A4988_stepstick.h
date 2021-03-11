#pragma once

#include <gmock/gmock.h>


class Mock_A4988_stepstick {
public:
    MOCK_METHOD1(motor_update_pins, void(Motor* motor));
    MOCK_METHOD1(motor_init, void(Motor* motor));
    MOCK_METHOD1(motor_update, Std_Err(Motor* motor));
    MOCK_METHOD1(motor_start, Std_Err(Motor* motor));
    MOCK_METHOD1(motor_stop, Std_Err(Motor* motor));
    MOCK_METHOD2(motor_set_counters, void(Motor* motor, MotorCounters* counters));
    MOCK_METHOD2(motor_set_direction, void(Motor* motor, unsigned int direction));
    MOCK_METHOD2(motor_set_position, Std_Err(Motor* motor, double position));
    MOCK_METHOD6(motor_get_linear_move_settings, Std_Err(Motor* motor, double move, double speed,
        const int ACCURACY, MotorCounters* counters, bool* direction));


    static Mock_A4988_stepstick* mock;
};

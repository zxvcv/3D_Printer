#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_HAL_Drivers.h"

using ::testing::_;

extern "C"
{
    #include "A4988_stepstick.h"
}

class A4988_stepstick_UT: public ::testing::Test
{
public:
    A4988_stepstick_UT()
    {
        mock_HAL_Drivers = new Mock_HAL_Drivers();
    }
    ~A4988_stepstick_UT()
    {
        delete mock_HAL_Drivers;
    }

    virtual void SetUp()
    {
        IO_Pin reset_pin;
        IO_Pin sleep_pin;
        IO_Pin direction_pin;
        IO_Pin step_pin;

        motor = (Motor*)malloc(sizeof(Motor));

        EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
            .Times(4);
        motor_init(motor, 1000, reset_pin, sleep_pin, direction_pin, step_pin);
        setup_motor_values(motor);
    }

    virtual void TearDown()
    {
        free(motor);
    }

    void setup_motor_values(Motor* motor);
    // bool compareDouble(double compared, double expected);


    Motor* motor;

    /* MOCKS */
    static Mock_HAL_Drivers* mock_HAL_Drivers;
};

Mock_HAL_Drivers* A4988_stepstick_UT::mock_HAL_Drivers;



/************************** TESTS **************************/

TEST_F(A4988_stepstick_UT, motor_update_pins_resetChange)
{
    motor->flags.reset = 0;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET))
        .Times(2);
    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET))
        .Times(2);

    motor_update_pins(motor);

    motor->flags.reset = 1;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET))
        .Times(1);
    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET))
        .Times(3);

    motor_update_pins(motor);
}

TEST_F(A4988_stepstick_UT, motor_update_pins_sleepChange)
{
    motor->flags.sleep = 0;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET))
        .Times(3);
    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET))
        .Times(1);

    motor_update_pins(motor);

    motor->flags.sleep = 1;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET))
        .Times(2);
    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET))
        .Times(2);

    motor_update_pins(motor);
}

TEST_F(A4988_stepstick_UT, motor_update_pins_directionChange)
{
    motor->flags.direction = COUNTER_CLOCKWISE;
    motor->flags.reversed = 0;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET))
        .Times(1);
    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET))
        .Times(3);

    motor_update_pins(motor);

    motor->flags.direction = CLOCKWISE;
    motor->flags.reversed = 0;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET))
        .Times(2);
    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET))
        .Times(2);

    motor_update_pins(motor);

    motor->flags.direction = COUNTER_CLOCKWISE;
    motor->flags.reversed = 1;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET))
        .Times(2);
    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET))
        .Times(2);

    motor_update_pins(motor);

    motor->flags.direction = CLOCKWISE;
    motor->flags.reversed = 1;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET))
        .Times(1);
    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET))
        .Times(3);

    motor_update_pins(motor);
}

TEST_F(A4988_stepstick_UT, motor_update_pins_stepChange)
{
    motor->flags.stepPhase = 0;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET))
        .Times(2);
    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET))
        .Times(2);

    motor_update_pins(motor);

    motor->flags.stepPhase = 1;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET))
        .Times(3);
    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET))
        .Times(1);

    motor_update_pins(motor);
}

TEST_F(A4988_stepstick_UT, motor_start)
{
    Std_Err stdErr;

    #define TEST_VALUES_LEN 8
    struct{
        MotorCounters counters;
        Std_Err expected_exit_val;
    }test_values[TEST_VALUES_LEN] = {
        { { 0, 0, 0 }, STD_PARAMETER_ERROR },
        { { 0, 0, 1 }, STD_PARAMETER_ERROR },
        { { 0, 1, 0 }, STD_PARAMETER_ERROR },
        { { 0, 1, 1 }, STD_PARAMETER_ERROR },
        { { 1, 0, 0 }, STD_PARAMETER_ERROR },
        { { 1, 0, 1 }, STD_PARAMETER_ERROR },
        { { 1, 1, 0 }, STD_PARAMETER_ERROR },
        { { 1, 1, 1 }, STD_OK }
    };


    for(int i=0; i<TEST_VALUES_LEN; ++i)
    {
        motor->counters.timer = test_values[i].counters.timer;
        motor->counters.timer_start = test_values[i].counters.timer_start;
        motor->counters.steps = test_values[i].counters.steps;

        if(test_values[i].expected_exit_val == STD_OK)
        {
            EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
                .Times(4);
        }

        stdErr = motor_start(motor);
        EXPECT_EQ(stdErr, test_values[i].expected_exit_val);

        if(test_values[i].expected_exit_val == STD_OK)
        {
            EXPECT_EQ(motor->flags.sleep, 0);
            EXPECT_EQ(motor->flags.isOn, 1);
        }
    }
}

TEST_F(A4988_stepstick_UT, motor_stop_ok)
{
    Std_Err stdErr;

    motor->flags.reset = 0;
    motor->flags.isOn = 1;
    motor->flags.sleep = 0;
    motor->flags.stepPhase = 0;
    motor->counters.steps = 0;
    motor->counters.timer = 0;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);

    stdErr = motor_stop(motor);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
    EXPECT_EQ(motor->flags.stepPhase, 0);
}

TEST_F(A4988_stepstick_UT, motor_stop_resetState)
{
    Std_Err stdErr;

    motor->flags.reset = 1;
    motor->flags.isOn = 1;
    motor->flags.sleep = 0;
    motor->flags.stepPhase = 0;
    motor->counters.steps = 0;
    motor->counters.timer = 0;

    stdErr = motor_stop(motor);
    EXPECT_EQ(stdErr, STD_ERROR);
    EXPECT_EQ(motor->flags.isOn, 1);
    EXPECT_EQ(motor->flags.sleep, 0);
}

TEST_F(A4988_stepstick_UT, motor_stop_wrongStepPhase)
{
    Std_Err stdErr;

    motor->flags.reset = 0;
    motor->flags.isOn = 1;
    motor->flags.sleep = 0;
    motor->flags.stepPhase = 0;
    motor->counters.steps = 0;
    motor->counters.timer = 0;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);

    stdErr = motor_stop(motor);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
    EXPECT_EQ(motor->flags.stepPhase, 0);
}

TEST_F(A4988_stepstick_UT, motor_stop_workInProgress)
{
    Std_Err stdErr;

    motor->flags.reset = 0;
    motor->flags.isOn = 1;
    motor->flags.sleep = 0;
    motor->flags.stepPhase = 0;
    motor->counters.steps = 10;
    motor->counters.timer = 100;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);

    stdErr = motor_stop(motor);
    EXPECT_EQ(stdErr, STD_INTERRUPTED_ERROR);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
    EXPECT_EQ(motor->flags.stepPhase, 0);
}

TEST_F(A4988_stepstick_UT, motor_update_timerDecrease)
{
    Std_Err stdErr;

    motor->counters.timer = 5;
    motor->counters.timer_start = 10;
    motor->counters.steps = 5;
    motor->flags.isOn = 1;
    motor->flags.reset = 0;
    motor->flags.sleep = 0;

    stdErr = motor_update(motor);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_EQ(motor->counters.timer, 4);
    EXPECT_EQ(motor->counters.timer_start, 10);
    EXPECT_EQ(motor->counters.steps, 5);
    EXPECT_EQ(motor->flags.isOn, 1);
    EXPECT_EQ(motor->flags.sleep, 0);
}

TEST_F(A4988_stepstick_UT, motor_update_stepDecrease)
{
    Std_Err stdErr;

    motor->counters.timer = 1;
    motor->counters.timer_start = 10;
    motor->counters.steps = 5;
    motor->flags.isOn = 1;
    motor->flags.reset = 0;
    motor->flags.sleep = 0;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);

    stdErr = motor_update(motor);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_EQ(motor->counters.timer, 10);
    EXPECT_EQ(motor->counters.timer_start, 10);
    EXPECT_EQ(motor->counters.steps, 4);
    EXPECT_EQ(motor->flags.isOn, 1);
    EXPECT_EQ(motor->flags.sleep, 0);
}

TEST_F(A4988_stepstick_UT, motor_update_end)
{
    Std_Err stdErr;

    motor->counters.timer = 1;
    motor->counters.timer_start = 10;
    motor->counters.steps = 1;
    motor->flags.isOn = 1;
    motor->flags.reset = 0;
    motor->flags.sleep = 0;

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);

    stdErr = motor_update(motor);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_EQ(motor->counters.timer, 10);
    EXPECT_EQ(motor->counters.timer_start, 10);
    EXPECT_EQ(motor->counters.steps, 0);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
}

TEST_F(A4988_stepstick_UT, motor_set_position_position_ok)
{
    Std_Err stdErr;

    motor->settings.step_size = 203;

    stdErr = motor_set_position(motor, 10.);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(motor->data.position, 49);
    EXPECT_EQ(motor->data.position_error, 53);
}

TEST_F(A4988_stepstick_UT, motor_set_position_position_error)
{
    Std_Err stdErr;

    motor->settings.step_size = 203;

    stdErr = motor_set_position(motor, (-10.0));
    EXPECT_EQ(stdErr, STD_PARAMETER_ERROR);

    EXPECT_EQ(motor->data.position, 0);
    EXPECT_EQ(motor->data.position_error, 0);
}

TEST_F(A4988_stepstick_UT, basic_work_2steps)
{
    Std_Err stdErr;

    motor->counters.timer = 1;
    motor->counters.timer_start = 1;
    motor->counters.steps = 2;

    motor->flags.isOn = 1;
    motor->flags.sleep = 0;

    EXPECT_EQ(motor->flags.reset, 0);
    EXPECT_EQ(motor->flags.stepPhase, 0);

    for(int i=0; i<2; ++i)
    {
        EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
            .Times(4);

        stdErr = motor_update(motor);
        EXPECT_EQ(stdErr, STD_OK) << "iteration: " << i << std::endl;
    }

    EXPECT_EQ(motor->counters.steps, 0);
    EXPECT_EQ(motor->flags.stepPhase, 0);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
}

TEST_F(A4988_stepstick_UT, stop_before_move_is_ended_half_step_move)
{
    Std_Err stdErr;

    motor->counters.timer = 1;
    motor->counters.timer_start = 1;
    motor->counters.steps = 4;

    EXPECT_EQ(motor->flags.reset, 0);
    EXPECT_EQ(motor->flags.stepPhase, 0);

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);
    stdErr = motor_start(motor);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);
    stdErr = motor_update(motor);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);
    stdErr = motor_stop(motor);
    EXPECT_EQ(stdErr, STD_INTERRUPTED_ERROR);

    EXPECT_EQ(motor->counters.steps, 3);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
}

TEST_F(A4988_stepstick_UT, stop_before_move_is_ended_full_step_move)
{
    Std_Err stdErr;

    motor->counters.timer = 1;
    motor->counters.timer_start = 1;
    motor->counters.steps = 4;

    EXPECT_EQ(motor->flags.reset, 0);
    EXPECT_EQ(motor->flags.stepPhase, 0);

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);
    stdErr = motor_start(motor);
    EXPECT_EQ(stdErr, STD_OK);
    for(int i=0; i<2; ++i)
    {
        EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
            .Times(4);
        stdErr = motor_update(motor);
        EXPECT_EQ(stdErr, STD_OK);
    }

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);
    stdErr = motor_stop(motor);
    EXPECT_EQ(stdErr, STD_INTERRUPTED_ERROR);

    EXPECT_EQ(motor->counters.steps, 2);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
}

TEST_F(A4988_stepstick_UT, stop_after_move_is_ended)
{
    Std_Err stdErr;

    motor->counters.timer = 1;
    motor->counters.timer_start = 1;
    motor->counters.steps = 4;

    EXPECT_EQ(motor->flags.reset, 0);
    EXPECT_EQ(motor->flags.stepPhase, 0);

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);
    stdErr = motor_start(motor);
    EXPECT_EQ(stdErr, STD_OK);
    for(int i=0; i<4; ++i)
    {
        EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
            .Times(4);
        stdErr = motor_update(motor);
        EXPECT_EQ(stdErr, STD_OK);
    }

    EXPECT_CALL(*mock_HAL_Drivers, HAL_GPIO_WritePin(_, _, _))
        .Times(4);
    stdErr = motor_stop(motor);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(motor->counters.steps, 0);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
}

TEST_F(A4988_stepstick_UT, linear_move_get_basic1)
{
    Std_Err stdErr;

    MotorCounters expected_countersOUT = {};
    bool expected_directionOUT = false;

    stdErr = motor_get_linear_move_settings(motor, 5., 1., ACCURACY,
        &expected_countersOUT, &expected_directionOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(expected_countersOUT.timer, expected_countersOUT.timer_start);
    EXPECT_EQ(expected_countersOUT.timer, 101);
    EXPECT_EQ(expected_countersOUT.steps, 50);
    EXPECT_EQ(expected_directionOUT, false);
}

TEST_F(A4988_stepstick_UT, linear_move_get_basic2)
{
    Std_Err stdErr;

    MotorCounters expected_countersOUT;
    bool expected_directionOUT;

    stdErr = motor_get_linear_move_settings(motor, 7., 10., ACCURACY,
        &expected_countersOUT, &expected_directionOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(expected_countersOUT.timer, expected_countersOUT.timer_start);
    EXPECT_EQ(expected_countersOUT.timer, 10);
    EXPECT_EQ(expected_countersOUT.steps, 68);
    EXPECT_EQ(expected_directionOUT, false);
}

TEST_F(A4988_stepstick_UT, linear_move_speed_out_of_range)
{
    Std_Err stdErr;

    MotorCounters expected_countersOUT = {};
    bool expected_directionOUT = false;

    stdErr = motor_get_linear_move_settings(motor, 5., motor->settings.max_speed + 1, ACCURACY,
        &expected_countersOUT, &expected_directionOUT);
    EXPECT_EQ(stdErr, STD_PARAMETER_ERROR);

    EXPECT_EQ(expected_countersOUT.timer, expected_countersOUT.timer_start);
    EXPECT_EQ(expected_countersOUT.timer, 0);
    EXPECT_EQ(expected_countersOUT.steps, 0);
    EXPECT_EQ(expected_directionOUT, false);
}

TEST_F(A4988_stepstick_UT, linear_move_move_minus_out_of_range)
{
    Std_Err stdErr;

    MotorCounters expected_countersOUT = {};
    bool expected_directionOUT = false;

    stdErr = motor_get_linear_move_settings(motor, motor->settings.position_zero - 1, 10., ACCURACY,
        &expected_countersOUT, &expected_directionOUT);
    EXPECT_EQ(stdErr, STD_PARAMETER_ERROR);

    EXPECT_EQ(expected_countersOUT.timer, expected_countersOUT.timer_start);
    EXPECT_EQ(expected_countersOUT.timer, 0);
    EXPECT_EQ(expected_countersOUT.steps, 0);
    EXPECT_EQ(expected_directionOUT, false);
}

TEST_F(A4988_stepstick_UT, linear_move_move_plus_out_of_range)
{
    Std_Err stdErr;

    MotorCounters expected_countersOUT = {};
    bool expected_directionOUT = false;

    stdErr = motor_get_linear_move_settings(motor, motor->settings.position_end + 1, 10., ACCURACY,
        &expected_countersOUT, &expected_directionOUT);
    EXPECT_EQ(stdErr, STD_PARAMETER_ERROR);

    EXPECT_EQ(expected_countersOUT.timer, expected_countersOUT.timer_start);
    EXPECT_EQ(expected_countersOUT.timer, 0);
    EXPECT_EQ(expected_countersOUT.steps, 0);
    EXPECT_EQ(expected_directionOUT, false);
}

TEST_F(A4988_stepstick_UT, linear_move_move_minus_on_the_border)
{
    Std_Err stdErr;

    MotorCounters expected_countersOUT = {};
    bool expected_directionOUT = false;

    motor->data.position = 10 * ACCURACY;

    stdErr = motor_get_linear_move_settings(motor, -10., 10., ACCURACY,
        &expected_countersOUT, &expected_directionOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(expected_countersOUT.timer, expected_countersOUT.timer_start);
    EXPECT_EQ(expected_countersOUT.timer, 10);
    EXPECT_EQ(expected_countersOUT.steps, 98);
    EXPECT_EQ(expected_directionOUT, true);
}

TEST_F(A4988_stepstick_UT, linear_move_move_plus_on_the_border)
{
    Std_Err stdErr;

    MotorCounters expected_countersOUT = {};
    bool expected_directionOUT = false;

    stdErr = motor_get_linear_move_settings(motor, 10., 10., ACCURACY,
        &expected_countersOUT, &expected_directionOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(expected_countersOUT.timer, expected_countersOUT.timer_start);
    EXPECT_EQ(expected_countersOUT.timer, 10);
    EXPECT_EQ(expected_countersOUT.steps, 98);
    EXPECT_EQ(expected_directionOUT, false);
}



/************************** PUBLIC FUNCTIONS **************************/

void A4988_stepstick_UT::setup_motor_values(Motor* motor)
{
    motor->settings.step_size = 203;
    motor->settings.max_speed = 50;
    motor->settings.position_zero = 0 * ACCURACY;
    motor->settings.position_end = 20 * ACCURACY;

    motor->data.position = 0;
    motor->data.position_error = 0;
}

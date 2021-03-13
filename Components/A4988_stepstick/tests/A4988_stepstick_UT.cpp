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

/*
TEST_F(A4988_stepstick_test, a4988_stepstick__BasicWork2Steps__test)
{
    bool ret;

    motor->changeTime = 1;
    motor->counters.changeTime = 1;
    motor->counters.stepLeft = 2; // 1 step

    motor->flags.sleep = 0;
    motor->flags.isOn = 1;

    EXPECT_EQ(motor->flags.reset, 0);
    EXPECT_EQ(motor->flags.stepPhase, LOW);

    for(int i=0; i<2; ++i)
    {
        EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);

        ret = motorUpdate(motor);
        EXPECT_TRUE(ret) << "iteration: " << i << std::endl;
    }

    EXPECT_EQ(motor->counters.stepLeft, 0);
    EXPECT_EQ(motor->flags.stepPhase, LOW);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
}

TEST_F(A4988_stepstick_test, a4988_stepstick__Start__test)
{
    bool ret;

    motor->changeTime = 1;
    motor->counters.changeTime = 1;
    motor->counters.stepLeft = 2; // 1 step

    EXPECT_EQ(motor->flags.reset, 0);
    EXPECT_EQ(motor->flags.stepPhase, LOW);

    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorStart(motor);
    EXPECT_FALSE(ret);
    EXPECT_EQ(motor->flags.sleep, 0);
    EXPECT_EQ(motor->flags.isOn, 1);
}

TEST_F(A4988_stepstick_test, a4988_stepstick__StopBeforeMoveIsEnded_HalfStepMove__test)
{
    bool ret;

    motor->changeTime = 1;
    motor->counters.changeTime = 1;
    motor->counters.stepLeft = 4; // 1 step

    EXPECT_EQ(motor->flags.reset, 0);
    EXPECT_EQ(motor->flags.stepPhase, LOW);

    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorStart(motor);
    EXPECT_FALSE(ret);
    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorUpdate(motor);
    EXPECT_TRUE(ret);

    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorStop(motor);
    EXPECT_FALSE(ret);
    EXPECT_EQ(motor->counters.stepLeft, 3);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
}

TEST_F(A4988_stepstick_test, a4988_stepstick__StopBeforeMoveIsEnded_FullStepMove__test)
{
    bool ret;

    motor->changeTime = 1;
    motor->counters.changeTime = 1;
    motor->counters.stepLeft = 4; // 1 step

    EXPECT_EQ(motor->flags.reset, 0);
    EXPECT_EQ(motor->flags.stepPhase, LOW);

    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorStart(motor);
    EXPECT_FALSE(ret);
    for(int i=0; i<2; ++i)
    {
        EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
        ret = motorUpdate(motor);
        EXPECT_TRUE(ret);
    }

    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorStop(motor);
    EXPECT_FALSE(ret);
    EXPECT_EQ(motor->counters.stepLeft, 2);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
}

TEST_F(A4988_stepstick_test, a4988_stepstick__StopAfterMoveIsEnded__test)
{
    bool ret;

    motor->changeTime = 1;
    motor->counters.changeTime = 1;
    motor->counters.stepLeft = 4; // 1 step

    EXPECT_EQ(motor->flags.reset, 0);
    EXPECT_EQ(motor->flags.stepPhase, LOW);

    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorStart(motor);
    EXPECT_FALSE(ret);
    for(int i=0; i<4; ++i)
    {
        EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
        ret = motorUpdate(motor);
        EXPECT_TRUE(ret);
    }

    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorStop(motor);
    EXPECT_FALSE(ret);
    EXPECT_EQ(motor->counters.stepLeft, 0);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
}

TEST_F(A4988_stepstick_test, a4988_stepstick__SettingMove_Basic1__test)
{
    bool ret;
    RoundingErrorData roundingError;

    double testMove = 5.;
    motor->data.position = 10 * ACCURACY;
    motor->data.speed = 10;
    motor->data.err.moveError = 0;
    motor->data.err.speedError = 0;

    ret = motorSetMove(motor, testMove, &roundingError);
    EXPECT_FALSE(ret);

    EXPECT_EQ(motor->counters.stepLeft, 50);
    EXPECT_EQ(motor->counters.changeTime, 10);
    EXPECT_EQ(motor->changeTime, motor->counters.changeTime);
    EXPECT_EQ(roundingError.moveError, ((int)-75));
    EXPECT_TRUE(compareDouble(roundingError.speedError, -0.15));
}

TEST_F(A4988_stepstick_test, a4988_stepstick__SettingMove_Basic2__test)
{

    bool ret;
    RoundingErrorData roundingError;

    double testMove = 7.;
    motor->data.position = 10 * ACCURACY;
    motor->data.speed = 10;
    motor->data.err.moveError = 0;
    motor->data.err.speedError = 0;

    ret = motorSetMove(motor, testMove, &roundingError);
    EXPECT_FALSE(ret);

    EXPECT_EQ(motor->counters.stepLeft, 68);
    EXPECT_EQ(motor->counters.changeTime, 10);
    EXPECT_EQ(motor->changeTime, motor->counters.changeTime);
    EXPECT_EQ(roundingError.moveError, ((int)98));
    EXPECT_TRUE(compareDouble(roundingError.speedError, -0.15));
}

TEST_F(A4988_stepstick_test, a4988_stepstick__SettingMove_SpeedOutOfRange__test)
{
    bool ret;
    RoundingErrorData roundingError;

    double testMove = 5.;
    motor->data.position = 10 * ACCURACY;
    motor->data.speed = 100;
    motor->data.err.moveError = 0;
    motor->data.err.speedError = 0;

    ret = motorSetMove(motor, testMove, &roundingError);
    EXPECT_TRUE(ret);

    EXPECT_EQ(motor->counters.stepLeft, 0);
    EXPECT_EQ(motor->counters.changeTime, 0);
    EXPECT_EQ(motor->changeTime, motor->counters.changeTime);
    EXPECT_EQ(roundingError.moveError, 0);
    EXPECT_TRUE(compareDouble(roundingError.speedError, 0));
}

TEST_F(A4988_stepstick_test, a4988_stepstick__SettingMove_MoveLowOutOfRange__test)
{
    bool ret;
    RoundingErrorData roundingError;

    double testMove = -15.;
    motor->data.position = 10 * ACCURACY;
    motor->data.speed = 10;
    motor->data.err.moveError = 0;
    motor->data.err.speedError = 0;

    ret = motorSetMove(motor, testMove, &roundingError);
    EXPECT_TRUE(ret);

    EXPECT_EQ(motor->counters.stepLeft, 0);
    EXPECT_EQ(motor->counters.changeTime, 0);
    EXPECT_EQ(motor->changeTime, motor->counters.changeTime);
    EXPECT_EQ(roundingError.moveError, 0);
    EXPECT_TRUE(compareDouble(roundingError.speedError, 0));
}

TEST_F(A4988_stepstick_test, a4988_stepstick__SettingMove_MoveHighOutOfRange__test)
{
    bool ret;
    RoundingErrorData roundingError;

    double testMove = 15.;
    motor->data.position = 10 * ACCURACY;
    motor->data.speed = 10;
    motor->data.err.moveError = 0;
    motor->data.err.speedError = 0;

    ret = motorSetMove(motor, testMove, &roundingError);
    EXPECT_TRUE(ret);

    EXPECT_EQ(motor->counters.stepLeft, 0);
    EXPECT_EQ(motor->counters.changeTime, 0);
    EXPECT_EQ(motor->changeTime, motor->counters.changeTime);
    EXPECT_EQ(roundingError.moveError, 0);
    EXPECT_TRUE(compareDouble(roundingError.speedError, 0));
}

TEST_F(A4988_stepstick_test, a4988_stepstick__SettingMove_MoveLowOnTheBorder__test)
{
    bool ret;
    RoundingErrorData roundingError;

    double testMove = -10.;
    motor->data.position = 10 * ACCURACY;
    motor->data.speed = 10;
    motor->data.err.moveError = 0;
    motor->data.err.speedError = 0;

    ret = motorSetMove(motor, testMove, &roundingError);
    EXPECT_FALSE(ret);

    EXPECT_EQ(motor->counters.stepLeft,98);
    EXPECT_EQ(motor->counters.changeTime, 10);
    EXPECT_EQ(motor->changeTime, motor->counters.changeTime);
    EXPECT_EQ(roundingError.moveError, ((int)-53));
    EXPECT_TRUE(compareDouble(roundingError.speedError, -0.15));
}

TEST_F(A4988_stepstick_test, a4988_stepstick__SettingMove_MoveHighOnTheBorder__test)
{
    bool ret;
    RoundingErrorData roundingError;

    double testMove = 10.;
    motor->data.position = 10 * ACCURACY;
    motor->data.speed = 10;
    motor->data.err.moveError = 0;
    motor->data.err.speedError = 0;

    ret = motorSetMove(motor, testMove, &roundingError);
    EXPECT_FALSE(ret);

    EXPECT_EQ(motor->counters.stepLeft, 98);
    EXPECT_EQ(motor->counters.changeTime, 10);
    EXPECT_EQ(motor->changeTime, motor->counters.changeTime);
    EXPECT_EQ(roundingError.moveError, ((int)53));
    EXPECT_TRUE(compareDouble(roundingError.speedError, -0.15));
}
*/


/************************** PUBLIC FUNCTIONS **************************/

void A4988_stepstick_UT::setup_motor_values(Motor* motor)
{
    motor->settings.step_size = 203;
    motor->settings.max_speed = 50;
    motor->settings.position_zero = 0 * ACCURACY;
    motor->settings.position_end = 20 * ACCURACY;
}
/*
bool A4988_stepstick_test::compareDouble(double compared, double expected)
{
    if(expected + ACCURACY > compared && expected - ACCURACY < compared)
    {
        return true;
    }
    else
    {
        return false;
    }
}
*/
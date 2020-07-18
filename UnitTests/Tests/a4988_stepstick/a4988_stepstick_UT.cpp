#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>

#define HIGH	1
#define LOW		0
#define ACCURACY 1000

using ::testing::_;

extern "C"
{
    #include "a4988_stepstick.h"
}



class Mock_A4988_stepstick {
public:
    MOCK_METHOD3(HAL_GPIO_WritePin, void(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState));
};

class A4988_stepstick_test : public ::testing::Test {
public:
    A4988_stepstick_test()
    {
        mock = new Mock_A4988_stepstick();
    }
    ~A4988_stepstick_test()
    {
        delete mock;
    }

    virtual void SetUp()
    {
        motor = (MotorSettings*)malloc(sizeof(MotorSettings));
        setupMotor1(motor);

        EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
        motorInit(motor);
    }

    virtual void TearDown()
    {
        free(motor);
    }

    void setupMotor1(MotorSettings* motor);
    bool compareDouble(double compared, double expected);


    MotorSettings* motor;
    
    static Mock_A4988_stepstick* mock;
};

Mock_A4988_stepstick* A4988_stepstick_test::mock;



/************************** TESTS **************************/

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
    EXPECT_TRUE(ret);
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
    EXPECT_TRUE(ret);
    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorUpdate(motor);
    EXPECT_TRUE(ret);

    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorStop(motor);
    EXPECT_TRUE(ret);
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
    EXPECT_TRUE(ret);
    for(int i=0; i<2; ++i)
    {
        EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
        ret = motorUpdate(motor);
        EXPECT_TRUE(ret);
    }
    
    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorStop(motor);
    EXPECT_TRUE(ret);
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
    EXPECT_TRUE(ret);
    for(int i=0; i<4; ++i)
    {
        EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
        ret = motorUpdate(motor);
        EXPECT_TRUE(ret);
    }
    
    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, _)).Times(4);
    ret = motorStop(motor);
    EXPECT_TRUE(ret);
    EXPECT_EQ(motor->counters.stepLeft, 0);
    EXPECT_EQ(motor->flags.isOn, 0);
    EXPECT_EQ(motor->flags.sleep, 1);
}

TEST_F(A4988_stepstick_test, a4988_stepstick__UpdatePins__test)
{
    bool ret;

    motor->flags.reset = 0;
    motor->flags.sleep = 0;
    motor->flags.direction = 0;
    motor->flags.stepPhase = 0;

    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET)).Times(2);
    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET)).Times(2);

    motorUpdatePins(motor);


    motor->flags.reset = 1;
    motor->flags.sleep = 1;
    motor->flags.direction = 1;
    motor->flags.stepPhase = 1;

    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, GPIO_PIN_RESET)).Times(2);
    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET)).Times(2);
    
    motorUpdatePins(motor);
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
    EXPECT_TRUE(ret);
    
    EXPECT_EQ(motor->counters.stepLeft, 50);
    EXPECT_EQ(motor->counters.changeTime, 10);
    EXPECT_EQ(motor->changeTime, motor->counters.changeTime);
    EXPECT_EQ(roundingError.moveError, ((int)-75));
    EXPECT_TRUE(compareDouble(roundingError.speedError, -0.15));
}

TEST_F(A4988_stepstick_test, a4988_stepstick__SettingMove_Basic2__test)
{
    /*TO DO*/
    bool ret;
    RoundingErrorData roundingError;

    double testMove = 7.;
    motor->data.position = 10 * ACCURACY;
    motor->data.speed = 10;
    motor->data.err.moveError = 0;
    motor->data.err.speedError = 0;

    ret = motorSetMove(motor, testMove, &roundingError);
    EXPECT_TRUE(ret);
    
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
    EXPECT_FALSE(ret);
    
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
    EXPECT_FALSE(ret);
    
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
    EXPECT_FALSE(ret);
    
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
    EXPECT_TRUE(ret);

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
    EXPECT_TRUE(ret);

    EXPECT_EQ(motor->counters.stepLeft, 98);
    EXPECT_EQ(motor->counters.changeTime, 10);
    EXPECT_EQ(motor->changeTime, motor->counters.changeTime);
    EXPECT_EQ(roundingError.moveError, ((int)53));
    EXPECT_TRUE(compareDouble(roundingError.speedError, -0.15));
}



/************************** PUBLIC FUNCTIONS **************************/

void A4988_stepstick_test::setupMotor1(MotorSettings* motor)
{
    motor->IOreset.PORT = MOT1_RESET_GPIO_Port;
    motor->IOreset.PIN = MOT1_RESET_Pin;
    motor->IOsleep.PORT = MOT1_SLEEP_GPIO_Port;
    motor->IOsleep.PIN = MOT1_SLEEP_Pin;
    motor->IOdirection.PORT = MOT1_DIRECTION_GPIO_Port;
    motor->IOdirection.PIN = MOT1_DIRECTION_Pin;
    motor->IOstep.PORT = MOT1_STEP_GPIO_Port;
    motor->IOstep.PIN = MOT1_STEP_Pin;

    motor->flags.isOn = 0;
    motor->flags.reset = 0;
    motor->flags.sleep = 0;
    motor->flags.stepPhase = 0;
    motor->flags.direction = 0;

    motor->device.motorNum = 1;
    motor->device.eepromDataAddress = 0x00; 
    motor->device.timerFrequency = 1000;
    motor->device.stepSize = 203;
    motor->device.isReversed = false;
    motor->device.maxSpeed = 50;
    motor->device.positionZero = 0 * ACCURACY;
    motor->device.positionEnd = 20 * ACCURACY;
}

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



/************************** MOCK DEFINITIONS **************************/

extern "C"
{
    void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
    {
        A4988_stepstick_test::mock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
    }
}


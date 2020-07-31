#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define ACCURACY 1000

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;
//using ::testing::WillRepeatedly;
//using ::testing::SetArrayArgument;


extern "C"
{
    #include "manager.h"
}



class Mock_interrupts {
public:
    //MOCK_METHOD3(HAL_GPIO_WritePin, void(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState));

};

class Interrupts_test : public ::testing::Test {
public:
    Interrupts_test()
    {
        mock = new Mock_interrupts();
    }
    ~Interrupts_test()
    {
        delete mock;
    }

    virtual void SetUp()
    {
        settings = (DeviceSettings*)malloc(sizeof(DeviceSettings));
        for(int i=0; i<MOTORS_NUM; ++i)
        {
            settings->motors[i] = (MotorSettings*)malloc(sizeof(MotorSettings));
        }
        
        setupDevice(settings);
    }

    virtual void TearDown()
    {
        Std_Err stdErr;

        for(int i=0; i<MOTORS_NUM; ++i)
        {
            free(settings->motors[i]);
        }
        free(settings);
    }


    void setupMotor(MotorSettings* motor);
    void setupDevice(DeviceSettings* settings);


    DeviceSettings* settings;

    static Mock_interrupts* mock;
};

Mock_interrupts* Interrupts_test::mock;



/************************** TESTS **************************/

TEST_F(Interrupts_test, Interrupts__all__test)
{
    EXPECT_TRUE(true);
    std::cout << "Not Implemented!" << std::endl;
}




/************************** PUBLIC FUNCTIONS **************************/

void Interrupts_test::setupMotor(MotorSettings* motor)
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

void Interrupts_test::setupDevice(DeviceSettings* settings)
{
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        setupMotor(settings->motors[i]);
    }
}



/************************** MOCK DEFINITIONS **************************/

extern "C"
{
    /*void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
    {
        Interrupts_test::mock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
    }*/
}


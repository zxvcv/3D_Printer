#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdint.h>
#include <string.h>

#define ACCURACY 1000

using ::testing::_;
using ::testing::Return;
//using ::testing::WillRepeatedly;
//using ::testing::SetArrayArgument;


extern "C"
{
    #include "outerCommunication.h"
    #include "ProjectObjects.h"
    #include "FIFO_void.h"
}



class Mock_outerCommunication {
public:
    MOCK_METHOD3(HAL_UART_Receive_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size));
    MOCK_METHOD3(HAL_UART_Transmit_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size));
    
};

class OuterCommunication_test : public ::testing::Test {
public:
    OuterCommunication_test()
    {
        mock = new Mock_outerCommunication();
    }
    ~OuterCommunication_test()
    {
        delete mock;
    }

    virtual void SetUp()
    {
        Std_Err stdErr;

        settings = (DeviceSettings*)malloc(sizeof(DeviceSettings));
        for(int i=0; i<MOTORS_NUM; ++i)
        {
            settings->motors[i] = (MotorSettings*)malloc(sizeof(MotorSettings));
        }
        settings->outComm = (OuterComm_Settings*)malloc(sizeof(OuterComm_Settings));
        
        setupDevice(settings);

        EXPECT_CALL(*mock, HAL_UART_Receive_IT(settings->outComm->huart, _, _))
                .WillOnce(Return(HAL_OK));
        stdErr = init_outer_operations(settings->outComm);
        EXPECT_EQ(stdErr, STD_OK);
    }

    virtual void TearDown()
    {
        Std_Err stdErr;

        stdErr = deinit_outer_operations(settings->outComm);
        EXPECT_EQ(stdErr, STD_OK);

        free(settings->outComm);
        for(int i=0; i<MOTORS_NUM; ++i)
        {
            free(settings->motors[i]);
        }
        free(settings);
    }


    void setupMotor(MotorSettings* motor);
    void setupOuterComm(OuterComm_Settings* settings);
    void setupDevice(DeviceSettings* settings);


    DeviceSettings* settings;

    static Mock_outerCommunication* mock;
};

Mock_outerCommunication* OuterCommunication_test::mock;



/************************** TESTS **************************/

TEST_F(OuterCommunication_test, outerCommunication__send_outer_command_noCommandsWaiting__test)
{
    Std_Err stdErr = STD_OK;

    EXPECT_EQ(fifo_getSize(settings->outComm->Buff_OUT), 0);
    
    EXPECT_EQ(settings->outComm->transmission, false);

    stdErr = send_outer_command(settings->outComm);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(fifo_getSize(settings->outComm->Buff_OUT), 0);
    
    EXPECT_EQ(settings->outComm->transmission, false);
}

TEST_F(OuterCommunication_test, outerCommunication__send_outer_command_oneCommandWaiting__test)
{
    Std_Err stdErr = STD_OK;

    char cmdStr[] = "SP M1 0.400000";

    stdErr = fifo_push_C(settings->outComm->Buff_OUT, cmdStr, strlen(cmdStr));
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(fifo_getSize(settings->outComm->Buff_OUT), 1);
    
    EXPECT_EQ(settings->outComm->transmission, false);

    EXPECT_CALL(*mock, HAL_UART_Transmit_IT(settings->outComm->huart, _, _))
            .WillOnce(Return(HAL_OK));
    stdErr = send_outer_command(settings->outComm);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(fifo_getSize(settings->outComm->Buff_OUT), 1);
    
    EXPECT_EQ(settings->outComm->transmission, true);
}



/************************** PUBLIC FUNCTIONS **************************/

void OuterCommunication_test::setupMotor(MotorSettings* motor)
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

void OuterCommunication_test::setupOuterComm(OuterComm_Settings* settings)
{
	settings->Buff_InputCommands = NULL;
	settings->Buff_IN = NULL;
	settings->Buff_OUT = NULL;
	settings->huart = &huart1;
	settings->EOL_recieved = false;
	settings->transmission = false;
}

void OuterCommunication_test::setupDevice(DeviceSettings* settings)
{
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        setupMotor(settings->motors[i]);
    }

    setupOuterComm(settings->outComm);
}



/************************** MOCK DEFINITIONS **************************/

extern "C"
{
    HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        OuterCommunication_test::mock->HAL_UART_Receive_IT(huart, pData, Size);
    }

    HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        OuterCommunication_test::mock->HAL_UART_Transmit_IT(huart, pData, Size);
    }
}


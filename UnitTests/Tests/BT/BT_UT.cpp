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
    #include "BT.h"
    #include "ProjectObjects.h"
    #include "FIFO_void.h"
}



class Mock_BT {
public:
    MOCK_METHOD3(HAL_UART_Receive_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size));
    MOCK_METHOD3(HAL_UART_Transmit_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size));
    
};

class BT_test : public ::testing::Test {
public:
    BT_test()
    {
        mock = new Mock_BT();
    }
    ~BT_test()
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
        settings->bt = (BT_Settings*)malloc(sizeof(BT_Settings));
        
        setupDevice(settings);

        EXPECT_CALL(*mock, HAL_UART_Receive_IT(settings->bt->huart, _, _))
                .WillOnce(Return(HAL_OK));
        stdErr = init_operations_BT(settings->bt);
        EXPECT_EQ(stdErr, STD_OK);
    }

    virtual void TearDown()
    {
        Std_Err stdErr;

        stdErr = deinit_operations_BT(settings->bt);
        EXPECT_EQ(stdErr, STD_OK);

        free(settings->bt);
        for(int i=0; i<MOTORS_NUM; ++i)
        {
            free(settings->motors[i]);
        }
        free(settings);
    }


    void setupMotor(MotorSettings* motor);
    void setupBT(BT_Settings* bt);
    void setupDevice(DeviceSettings* settings);


    DeviceSettings* settings;

    static Mock_BT* mock;
};

Mock_BT* BT_test::mock;



/************************** TESTS **************************/

TEST_F(BT_test, BT__send_command_BT_noCommandsWaiting__test)
{
    Std_Err stdErr = STD_OK;

    EXPECT_EQ(fifo_getSize(settings->bt->Buff_Bt_OUT), 0);
    
    EXPECT_EQ(settings->bt->transmissionBT, false);

    stdErr = send_command_BT(settings->bt);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(fifo_getSize(settings->bt->Buff_Bt_OUT), 0);
    
    EXPECT_EQ(settings->bt->transmissionBT, false);
}

TEST_F(BT_test, BT__send_command_BT_oneCommandWaiting__test)
{
    Std_Err stdErr = STD_OK;

    char cmdStr[] = "SP M1 0.400000";

    stdErr = fifo_push_C(settings->bt->Buff_Bt_OUT, cmdStr, strlen(cmdStr));
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(fifo_getSize(settings->bt->Buff_Bt_OUT), 1);
    
    EXPECT_EQ(settings->bt->transmissionBT, false);

    EXPECT_CALL(*mock, HAL_UART_Transmit_IT(settings->bt->huart, _, _))
            .WillOnce(Return(HAL_OK));
    stdErr = send_command_BT(settings->bt);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(fifo_getSize(settings->bt->Buff_Bt_OUT), 1);
    
    EXPECT_EQ(settings->bt->transmissionBT, true);
}



/************************** PUBLIC FUNCTIONS **************************/

void BT_test::setupMotor(MotorSettings* motor)
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

void BT_test::setupBT(BT_Settings* bt)
{
	bt->Buff_InputCommandsBT = NULL;
	bt->Buff_Bt_IN = NULL;
	bt->Buff_Bt_OUT = NULL;
	bt->huart = &huart1;
	bt->EOL_BT_recieved = false;
	bt->transmissionBT = false;
}

void BT_test::setupDevice(DeviceSettings* settings)
{
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        setupMotor(settings->motors[i]);
    }

    setupBT(settings->bt);
}



/************************** MOCK DEFINITIONS **************************/

extern "C"
{
    HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        BT_test::mock->HAL_UART_Receive_IT(huart, pData, Size);
    }

    HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        BT_test::mock->HAL_UART_Transmit_IT(huart, pData, Size);
    }
}


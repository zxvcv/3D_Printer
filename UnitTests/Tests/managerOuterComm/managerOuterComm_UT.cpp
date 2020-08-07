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
    #include "managerOuterComm.h"
    #include "outerCommunication.h"
    #include "parserCommand.h"
}



class Mock_managerOuterComm {
public:
    MOCK_METHOD3(HAL_UART_Receive_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size));
    MOCK_METHOD3(HAL_UART_Transmit_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size));
    
    MOCK_METHOD3(HAL_GPIO_WritePin, void(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState));
    MOCK_METHOD7(HAL_I2C_Mem_Write, HAL_StatusTypeDef(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
        uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout));
    MOCK_METHOD7(HAL_I2C_Mem_Read, HAL_StatusTypeDef(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
        uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout));
    MOCK_METHOD3(f_open, FRESULT(FIL* fp, const TCHAR* path, BYTE mode));
    MOCK_METHOD3(f_mount, FRESULT(FATFS* fs, const TCHAR* path, BYTE opt));
    MOCK_METHOD4(EEPROM_writeData, Std_Err(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size));
};

class ManagerOuterComm_test : public ::testing::Test {
public:
    ManagerOuterComm_test()
    {
        mock = new Mock_managerOuterComm();
    }
    ~ManagerOuterComm_test()
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

    static Mock_managerOuterComm* mock;
};

Mock_managerOuterComm* ManagerOuterComm_test::mock;



/************************** TESTS **************************/

TEST_F(ManagerOuterComm_test, ManagerOuterComm__parse_execute_outer_data__test)
{
    Std_Err stdErr = STD_OK;

    uint8_t queueSize;
    char* sendCmd;

    SystemCommand* cmd = NULL;
    char cmdStr[] = "SP M1 0.400000\n";
    uint8_t cmdLen = 15;

    /*parse*/
    for(int i=0; i<cmdLen; ++i)
    {
        stdErr = fifo_push_C(settings->outComm->Buff_IN, &(cmdStr[i]), 1);
        EXPECT_EQ(stdErr, STD_OK);
    }

    settings->EOL_recieved = true;
    stdErr = parse_outer_data(settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    ASSERT_GT(fifo_getSize(settings->outComm->Buff_InputCommands), 0);
    stdErr = fifo_front(settings->outComm->Buff_InputCommands, (void**)&cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(cmd->motorsNum, 1);
    EXPECT_EQ(cmd->motor[0], settings->motors[0]);
    EXPECT_FLOAT_EQ(cmd->arg[0], 0.400000);
    EXPECT_FLOAT_EQ(cmd->arg[1], 0);

    /*execute*/
    EXPECT_CALL(*mock, EEPROM_writeData(settings->eeprom, _, _, _));
    stdErr = execute_outer_command(settings);
    EXPECT_EQ(stdErr, STD_OK);

    
    EXPECT_EQ(fifo_getSize(settings->outComm->Buff_OUT), 1);
    EXPECT_EQ(fifo_getSize(settings->outComm->Buff_IN), 0);
    EXPECT_EQ(fifo_getSize(settings->outComm->Buff_InputCommands), 0);

    stdErr = fifo_front(settings->outComm->Buff_OUT, (void**)&sendCmd);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_STREQ(sendCmd, "SP 0.400000 0.203000 0.203000 0.203000 0.000000\n");
}



/************************** PUBLIC FUNCTIONS **************************/

void ManagerOuterComm_test::setupMotor(MotorSettings* motor)
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

void ManagerOuterComm_test::setupOuterComm(OuterComm_Settings* settings)
{
	settings->Buff_InputCommands = NULL;
	settings->Buff_IN = NULL;
	settings->Buff_OUT = NULL;
	settings->huart = &huart1;
	settings->EOL_recieved = false;
	settings->transmission = false;
}

void ManagerOuterComm_test::setupDevice(DeviceSettings* settings)
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
        ManagerOuterComm_test::mock->HAL_UART_Receive_IT(huart, pData, Size);
    }

    HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        ManagerOuterComm_test::mock->HAL_UART_Transmit_IT(huart, pData, Size);
    }

    void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
    {
        ManagerOuterComm_test::mock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
    }

    HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
        uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
    {
        ManagerOuterComm_test::mock->HAL_I2C_Mem_Write(hi2c, DevAddress, MemAddress, MemAddSize, 
            pData, Size, Timeout);
    }

    HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
        uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
    {
        ManagerOuterComm_test::mock->HAL_I2C_Mem_Read(hi2c, DevAddress, MemAddress,  MemAddSize, 
            pData, Size, Timeout);
    }

    FRESULT f_open(FIL* fp, const TCHAR* path,	BYTE mode)
    {
        ManagerOuterComm_test::mock->f_open(fp, path, mode);
    }

    FRESULT f_mount(FATFS* fs,	const TCHAR* path, BYTE opt)
    {
        ManagerOuterComm_test::mock->f_mount(fs, path, opt);
    }

    Std_Err EEPROM_writeData(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size)
    {
        ManagerOuterComm_test::mock->EEPROM_writeData(settings, address, data, size);
    }
}


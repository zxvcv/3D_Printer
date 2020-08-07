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



class Mock_manager {
public:
    MOCK_METHOD3(HAL_GPIO_WritePin, void(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState));
    MOCK_METHOD4(EEPROM_readData, Std_Err(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size));
    MOCK_METHOD4(EEPROM_writeData, Std_Err(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size));
    MOCK_METHOD1(init_outer_operations, Std_Err(OuterComm_Settings* settings));
    MOCK_METHOD1(init_operations_SDcard, Std_Err(SDCard_Settings* settings));
};

class Manager_test : public ::testing::Test {
public:
    Manager_test()
    {
        mock = new Mock_manager();
    }
    ~Manager_test()
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

    static Mock_manager* mock;
};

Mock_manager* Manager_test::mock;



/************************** TESTS **************************/

TEST_F(Manager_test, Manager__init_manager__test)
{
    Std_Err stdErr;

    EXPECT_CALL(*mock, init_outer_operations(settings->outComm))
        .WillOnce(Return(STD_OK));
    EXPECT_CALL(*mock, HAL_GPIO_WritePin(_, _, GPIO_PIN_SET));
    EXPECT_CALL(*mock, init_operations_SDcard(settings->sd))
        .WillOnce(Return(STD_OK));

    stdErr = init_manager(settings);
    EXPECT_EQ(stdErr, STD_OK);
}

TEST_F(Manager_test, Manager__clearAllMotorsRoundingErrors__test)
{
    Std_Err stdErr;

    stdErr = clearAllMotorsRoundingErrors(settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(settings->motors[0]->data.err.moveError, 0.0);
    EXPECT_FLOAT_EQ(settings->motors[0]->data.err.speedError, 0.0);
    EXPECT_FLOAT_EQ(settings->motors[1]->data.err.moveError, 0.0);
    EXPECT_FLOAT_EQ(settings->motors[1]->data.err.speedError, 0.0);
    EXPECT_FLOAT_EQ(settings->motors[2]->data.err.moveError, 0.0);
    EXPECT_FLOAT_EQ(settings->motors[2]->data.err.speedError, 0.0);
    EXPECT_FLOAT_EQ(settings->motors[3]->data.err.moveError, 0.0);
    EXPECT_FLOAT_EQ(settings->motors[3]->data.err.speedError, 0.0);
}

TEST_F(Manager_test, Manager__getMotorData_EEPROM__test)
{
    Std_Err stdErr;

    MotorData_EEPROM data;
    data.stepSize = 0.203;
    data.maxSpeed = 1.0;
    data.positionZero = 2.0;
    data.positionEnd = 3.0;

    EXPECT_CALL(*mock, EEPROM_readData(settings->eeprom, 
        settings->motors[0]->device.eepromDataAddress, _, _));
    //    .WillOnce(DoAll(SetArgPointee<2>(data), Return(STD_OK)));
    stdErr = getMotorData_EEPROM(settings->motors[0], settings->eeprom);
    EXPECT_EQ(stdErr, STD_OK);

    //EXPECT_FLOAT_EQ(settings->motors[0]->device.stepSize, 0.203);
    //EXPECT_FLOAT_EQ(settings->motors[0]->device.maxSpeed, 1.0);
    //EXPECT_FLOAT_EQ(settings->motors[0]->device.positionZero, 2.0);
    //EXPECT_FLOAT_EQ(settings->motors[0]->device.positionEnd, 3.0);
}

TEST_F(Manager_test, Manager__setMotorData_EEPROM__test)
{
    Std_Err stdErr;

    MotorData_EEPROM data;
    data.stepSize = 0.203;
    data.maxSpeed = 1.0;
    data.positionZero = 2.0;
    data.positionEnd = 3.0;

    EXPECT_CALL(*mock, EEPROM_writeData(settings->eeprom, 
        settings->motors[0]->device.eepromDataAddress, (uint8_t*)(&data), _));

    stdErr = setMotorData_EEPROM(settings->motors[0], settings->eeprom, &data);
    EXPECT_EQ(stdErr, STD_OK);
}



/************************** PUBLIC FUNCTIONS **************************/

void Manager_test::setupMotor(MotorSettings* motor)
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

void Manager_test::setupDevice(DeviceSettings* settings)
{
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        setupMotor(settings->motors[i]);
    }
}



/************************** MOCK DEFINITIONS **************************/

extern "C"
{
    void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
    {
        Manager_test::mock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
    }

    Std_Err EEPROM_readData(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size)
    {
        Manager_test::mock->EEPROM_readData(settings, address, data, size);
    }

    Std_Err EEPROM_writeData(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size)
    {
        Manager_test::mock->EEPROM_writeData(settings, address, data, size);
    }

    Std_Err init_outer_operations(OuterComm_Settings* settings)
    {
        Manager_test::mock->init_outer_operations(settings);
    }

    Std_Err init_operations_SDcard(SDCard_Settings* settings)
    {
        Manager_test::mock->init_operations_SDcard(settings);
    }
}


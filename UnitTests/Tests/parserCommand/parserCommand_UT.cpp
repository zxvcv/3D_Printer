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
    #include "parserCommand.h"
    #include "managerBT.h"
}



class Mock_parserCommand {
public:
    MOCK_METHOD3(HAL_GPIO_WritePin, void(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState));
    MOCK_METHOD7(HAL_I2C_Mem_Write, HAL_StatusTypeDef(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
        uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout));
    MOCK_METHOD7(HAL_I2C_Mem_Read, HAL_StatusTypeDef(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
        uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout));
    MOCK_METHOD3(f_open, FRESULT(FIL* fp, const TCHAR* path, BYTE mode));
    MOCK_METHOD3(f_mount, FRESULT(FATFS* fs, const TCHAR* path, BYTE opt));
    MOCK_METHOD4(EEPROM_writeData, Std_Err(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size));

    MOCK_METHOD3(motorSetMove, bool(MotorSettings* settings, double move, RoundingErrorData* roundingError));
    MOCK_METHOD1(motorStart, bool(MotorSettings* settings));
    MOCK_METHOD1(motorIsOn, bool(MotorSettings* settings));
};

class ParserCommand_test : public ::testing::Test {
public:
    ParserCommand_test()
    {
        mock = new Mock_parserCommand();
    }
    ~ParserCommand_test()
    {
        delete mock;
    }

    virtual void SetUp()
    {
        Fifo_Err fifoErr;

        settings = (DeviceSettings*)malloc(sizeof(DeviceSettings));
        for(int i=0; i<MOTORS_NUM; ++i)
        {
            settings->motors[i] = (MotorSettings*)malloc(sizeof(MotorSettings));
        }
        settings->bt = (BT_Settings*)malloc(sizeof(BT_Settings));
        settings->eeprom = (EEPROMSettings*)malloc(sizeof(EEPROMSettings));
        settings->sd = (SDCard_Settings*)malloc(sizeof(SDCard_Settings));
        
        setupDevice(settings);

        //init_operations_BT(settings->bt);
        list_create(&(settings->bt->Buff_InputCommandsBT), &fifoErr);
        EXPECT_EQ(fifoErr, QUEUE_OK);
	    list_create(&(settings->bt->Buff_Bt_IN), &fifoErr);
        EXPECT_EQ(fifoErr, QUEUE_OK);
	    list_create(&(settings->bt->Buff_Bt_OUT), &fifoErr);
        EXPECT_EQ(fifoErr, QUEUE_OK);
    }

    virtual void TearDown()
    {
        Fifo_Err fifoErr;

        //deinit_operations_BT(settings->bt);
        list_delete_C(&(settings->bt->Buff_InputCommandsBT), &fifoErr);
        EXPECT_EQ(fifoErr, QUEUE_OK);
        list_delete_C(&(settings->bt->Buff_Bt_IN), &fifoErr);
        EXPECT_EQ(fifoErr, QUEUE_OK);
        list_delete_C(&(settings->bt->Buff_Bt_OUT), &fifoErr);
        EXPECT_EQ(fifoErr, QUEUE_OK);
        
        free(settings->sd);
        free(settings->eeprom);
        free(settings->bt);
        for(int i=0; i<MOTORS_NUM; ++i)
        {
            free(settings->motors[i]);
        }
        free(settings);
    }


    void setupMotor(MotorSettings* motor);
    void setupBT(BT_Settings* bt);
    void setupEEPROM(EEPROMSettings* eeprom);
    void setupSD(SDCard_Settings* sd);
    void setupDevice(DeviceSettings* settings);


    DeviceSettings* settings;

    static Mock_parserCommand* mock;
};

Mock_parserCommand* ParserCommand_test::mock;



/************************** TESTS **************************/

TEST_F(ParserCommand_test, ParserCommand__MotorDataRequest_SingleMotor_Correct__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "DR M1";

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(cmd.motorsNum, 1);
    EXPECT_EQ(cmd.motor[0], settings->motors[0]);
    EXPECT_FLOAT_EQ(cmd.arg[0], 0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 1);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);

    sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_STREQ(sendCmd, "DT M1 0.000000 0.000000 20.000000 0.000000 50.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__MotorDataRequest_MultipleMotor_Correct__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "DR M13";

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(cmd.motorsNum, 2);
    EXPECT_EQ(cmd.motor[0], settings->motors[0]);
    EXPECT_EQ(cmd.motor[1], settings->motors[2]);
    EXPECT_FLOAT_EQ(cmd.arg[0], 0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 2);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);

    sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_STREQ(sendCmd, "DT M1 0.000000 0.000000 20.000000 0.000000 50.000000\n");
    list_pop_C(settings->bt->Buff_Bt_OUT, &fifoErr);
    sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_STREQ(sendCmd, "DT M1 0.000000 0.000000 20.000000 0.000000 50.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__MotorPositionMove_SingleMotor_Correct__test)
{
    /*TODO: make thise test better*/
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "PM M1 5";
    settings->motors[0]->data.position = 0;
    //settings->motors[0]->data.speed = 10.0;
    settings->motors[0]->data.speed = 0.0;

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(cmd.motorsNum, 1);
    EXPECT_EQ(cmd.motor[0], settings->motors[0]);
    EXPECT_FLOAT_EQ(cmd.arg[0], 5.0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    EXPECT_CALL(*mock, motorSetMove(settings->motors[0], 5.0, _)).WillOnce(Return(false));
    EXPECT_CALL(*mock, motorStart(settings->motors[0])).WillOnce(Return(false));
    EXPECT_CALL(*mock, motorIsOn(settings->motors[0])).WillOnce(Return(false));
    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    //queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    //EXPECT_EQ(queueSize, 1);
    //queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    //EXPECT_EQ(queueSize, 0);
    //queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    //EXPECT_EQ(queueSize, 0);

    //sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    //std::cout << sendCmd << std::endl;
    //EXPECT_STREQ(sendCmd, "DT M1 0.000000 0.000000 20.000000 0.000000 50.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__MotorPositionValueSet_SingleMotor_Correct__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "PV M1 10.000000";
    settings->motors[0]->data.position = 0;
    settings->motors[0]->data.speed = 10.0;

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(cmd.motorsNum, 1);
    EXPECT_EQ(cmd.motor[0], settings->motors[0]);
    EXPECT_FLOAT_EQ(cmd.arg[0], 10.0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 1);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);

    sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_STREQ(sendCmd, "DT M1 10.000000 0.000000 20.000000 10.000000 50.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__MotorPositionZero_SingleMotor_Correct__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "PZ M1 10.000000";
    settings->motors[0]->data.position = 0;
    settings->motors[0]->data.speed = 10.0;

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(cmd.motorsNum, 1);
    EXPECT_EQ(cmd.motor[0], settings->motors[0]);
    EXPECT_FLOAT_EQ(cmd.arg[0], 10.0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    EXPECT_CALL(*mock, EEPROM_writeData(settings->eeprom, _, _, _));
    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 1);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);

    sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_STREQ(sendCmd, "DT M1 0.000000 10.000000 20.000000 10.000000 50.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__MotorPositionEnd_SingleMotor_Correct__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "PE M1 30";
    settings->motors[0]->data.position = 0;
    settings->motors[0]->data.speed = 10.0;

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(cmd.motorsNum, 1);
    EXPECT_EQ(cmd.motor[0], settings->motors[0]);
    EXPECT_FLOAT_EQ(cmd.arg[0], 30.0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    EXPECT_CALL(*mock, EEPROM_writeData(settings->eeprom, _, _, _));
    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 1);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);

    sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_STREQ(sendCmd, "DT M1 0.000000 0.000000 30.000000 10.000000 50.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__MotorDistanceMove_SingleMotor_Correct__test)
{
    /*TODO: make thise test better*/
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "DM M1 5";
    settings->motors[0]->data.position = 0;
    //settings->motors[0]->data.speed = 10.0;
    settings->motors[0]->data.speed = 0.0;

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(cmd.motorsNum, 1);
    EXPECT_EQ(cmd.motor[0], settings->motors[0]);
    EXPECT_FLOAT_EQ(cmd.arg[0], 5.0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    EXPECT_CALL(*mock, motorSetMove(settings->motors[0], 5.0, _)).WillOnce(Return(false));
    EXPECT_CALL(*mock, motorStart(settings->motors[0])).WillOnce(Return(false));
    EXPECT_CALL(*mock, motorIsOn(settings->motors[0])).WillOnce(Return(false));
    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    //queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    //EXPECT_EQ(queueSize, 1);
    //queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    //EXPECT_EQ(queueSize, 0);
    //queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    //EXPECT_EQ(queueSize, 0);

    //sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    //std::cout << sendCmd << std::endl;
    //EXPECT_STREQ(sendCmd, "DT M1 0.000000 0.000000 20.000000 0.000000 50.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__MotorSpeedSet_SingleMotor_Correct__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "SS M1 20";
    settings->motors[0]->data.position = 0;
    settings->motors[0]->data.speed = 10.0;

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(cmd.motorsNum, 1);
    EXPECT_EQ(cmd.motor[0], settings->motors[0]);
    EXPECT_FLOAT_EQ(cmd.arg[0], 20.0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 1);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);

    sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_STREQ(sendCmd, "DT M1 0.000000 0.000000 20.000000 20.000000 50.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__MotorSpeedMax_SingleMotor_Correct__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "SM M1 40";
    settings->motors[0]->data.position = 0;
    settings->motors[0]->data.speed = 10.0;

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(cmd.motorsNum, 1);
    EXPECT_EQ(cmd.motor[0], settings->motors[0]);
    EXPECT_FLOAT_EQ(cmd.arg[0], 40.0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    EXPECT_CALL(*mock, EEPROM_writeData(settings->eeprom, _, _, _));
    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 1);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);

    sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_STREQ(sendCmd, "DT M1 0.000000 0.000000 20.000000 10.000000 40.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__MotorsStepSizeRequest_SingleMotor_Correct__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "SR";
    settings->motors[0]->data.position = 0;
    settings->motors[0]->data.speed = 10.0;

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.arg[0], 0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 1);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);

    sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_STREQ(sendCmd, "SP 0.203000 0.203000 0.203000 0.203000 0.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__MotorsStepSizeSet_SingleMotor_Correct__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "SP M1 0.400000";
    settings->motors[0]->data.position = 0;
    settings->motors[0]->data.speed = 10.0;

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.arg[0], 0.400);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    EXPECT_CALL(*mock, EEPROM_writeData(settings->eeprom, _, _, _));
    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 1);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);

    sendCmd = (char*)list_front(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_STREQ(sendCmd, "SP 0.400000 0.203000 0.203000 0.203000 0.000000\n");
}

TEST_F(ParserCommand_test, ParserCommand__SDCardProgramRun_SingleMotor_Correct__test)
{
    /*TODO: add LOG_ENABLE version*/

    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "CR";
    settings->motors[0]->data.position = 0;
    settings->motors[0]->data.speed = 10.0;

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.arg[0], 0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    EXPECT_CALL(*mock, f_mount(_, _, _));
    EXPECT_CALL(*mock, f_open(_, _, _));
    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);
}

TEST_F(ParserCommand_test, ParserCommand__WrongCommand__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;
    uint8_t queueSize;
    char* sendCmd;

    SystemCommand cmd;
    char cmdStr[] = "AAA BBB CCC DDD";

    stdErr = parseSystemCommand(cmdStr, &cmd, settings);
    EXPECT_EQ(stdErr, STD_ERROR);

    EXPECT_EQ(cmd.motorsNum, 0);
    EXPECT_FLOAT_EQ(cmd.arg[0], 0);
    EXPECT_FLOAT_EQ(cmd.arg[1], 0);

    stdErr = executeSystemCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_PARAMETER_ERROR);
    
    queueSize = list_getSize(settings->bt->Buff_Bt_OUT, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_Bt_IN, &fifoErr);
    EXPECT_EQ(queueSize, 0);
    queueSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);
    EXPECT_EQ(queueSize, 0);
}



/************************** PUBLIC FUNCTIONS **************************/

void ParserCommand_test::setupMotor(MotorSettings* motor)
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

void ParserCommand_test::setupBT(BT_Settings* bt)
{
	bt->Buff_InputCommandsBT = NULL;
	bt->Buff_Bt_IN = NULL;
	bt->Buff_Bt_OUT = NULL;
	bt->huart = &huart1;
	bt->EOL_BT_recieved = false;
	bt->transmissionBT = false;
}

void ParserCommand_test::setupEEPROM(EEPROMSettings* eeprom)
{
	eeprom->isReady = true;
    eeprom->i2c = &hi2c1;
}

void ParserCommand_test::setupSD(SDCard_Settings* sd)
{
	sd->end_SDprogram = false;
    sd->executing_SDcommand = false;
    sd->executing_SDprogram = false;
}

void ParserCommand_test::setupDevice(DeviceSettings* settings)
{
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        setupMotor(settings->motors[i]);
    }

    setupBT(settings->bt);
    setupEEPROM(settings->eeprom);
    setupSD(settings->sd);
}



/************************** MOCK DEFINITIONS **************************/

extern "C"
{
    void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
    {
        ParserCommand_test::mock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
    }

    HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
        uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
    {
        ParserCommand_test::mock->HAL_I2C_Mem_Write(hi2c, DevAddress, MemAddress, MemAddSize, 
            pData, Size, Timeout);
    }

    HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
        uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
    {
        ParserCommand_test::mock->HAL_I2C_Mem_Read(hi2c, DevAddress, MemAddress,  MemAddSize, 
            pData, Size, Timeout);
    }

    FRESULT f_open(FIL* fp, const TCHAR* path,	BYTE mode)
    {
        ParserCommand_test::mock->f_open(fp, path, mode);
    }

    FRESULT f_mount(FATFS* fs,	const TCHAR* path, BYTE opt)
    {
        ParserCommand_test::mock->f_mount(fs, path, opt);
    }

    Std_Err EEPROM_writeData(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size)
    {
        ParserCommand_test::mock->EEPROM_writeData(settings, address, data, size);
    }

    bool motorSetMove(MotorSettings* settings, double move, RoundingErrorData* roundingError)
    {
        ParserCommand_test::mock->motorSetMove(settings, move, roundingError);
    }

    bool motorStart(MotorSettings* settings)
    {
        ParserCommand_test::mock->motorStart(settings);
    }

    bool motorIsOn(MotorSettings* settings)
    {
        ParserCommand_test::mock->motorIsOn(settings);
    }
}


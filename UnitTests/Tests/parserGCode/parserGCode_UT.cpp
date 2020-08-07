#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdint.h>
#include <string.h>

#define ACCURACY 1000


using ::testing::_;
using ::testing::Return;


extern "C"
{
    #include "parserGCode.h"
    #include "ProjectTypes.h"
}

enum {
		RELATIVE,
		ABSOLUTE
}positioningMode;


class Mock_parserGCode {
public:
    MOCK_METHOD3(HAL_GPIO_WritePin, void(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState));

    MOCK_METHOD3(motorSetMove, bool(MotorSettings* settings, double move, RoundingErrorData* roundingError));
    MOCK_METHOD1(motorStart, bool(MotorSettings* settings));
    MOCK_METHOD1(motorIsOn, bool(MotorSettings* settings));
};

class ParserGCode_test : public ::testing::Test {
public:
    ParserGCode_test()
    {
        mock = new Mock_parserGCode();
    }
    ~ParserGCode_test()
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
        settings->eeprom = (EEPROMSettings*)malloc(sizeof(EEPROMSettings));
        settings->sd = (SDCard_Settings*)malloc(sizeof(SDCard_Settings));
        
        setupDevice(settings);

        //init_outer_operations(settings->outComm);
        stdErr = fifo_create(&(settings->outComm->Buff_InputCommands));
        EXPECT_EQ(stdErr, STD_OK);
	    stdErr = fifo_create(&(settings->outComm->Buff_IN));
        EXPECT_EQ(stdErr, STD_OK);
	    stdErr = fifo_create(&(settings->outComm->Buff_OUT));
        EXPECT_EQ(stdErr, STD_OK);
    }

    virtual void TearDown()
    {
        Std_Err stdErr;

        //deinit_outer_operations(settings->outComm);
        stdErr = fifo_delete_C(&(settings->outComm->Buff_InputCommands));
        EXPECT_EQ(stdErr, STD_OK);
        stdErr = fifo_delete_C(&(settings->outComm->Buff_IN));
        EXPECT_EQ(stdErr, STD_OK);
        stdErr = fifo_delete_C(&(settings->outComm->Buff_OUT));
        EXPECT_EQ(stdErr, STD_OK);
        
        free(settings->sd);
        free(settings->eeprom);
        free(settings->outComm);
        for(int i=0; i<MOTORS_NUM; ++i)
        {
            free(settings->motors[i]);
        }
        free(settings);
    }

    void setupMotor(MotorSettings* motor);
    void setupOuterComm(OuterComm_Settings* settings);
    void setupEEPROM(EEPROMSettings* eeprom);
    void setupSD(SDCard_Settings* sd);
    void setupDevice(DeviceSettings* settings);


    DeviceSettings* settings;

    static Mock_parserGCode* mock;
};

Mock_parserGCode* ParserGCode_test::mock;



/************************** TESTS **************************/

TEST_F(ParserGCode_test, ParserGCode__command_G1__test)
{
    /*TODO: make this test better*/
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "G1 X10 Y15 Z20 E5 F10";


    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.data.x, 10);     EXPECT_EQ(cmd.usedFields._x, 1);
    EXPECT_FLOAT_EQ(cmd.data.y, 15);     EXPECT_EQ(cmd.usedFields._y, 1);
    EXPECT_FLOAT_EQ(cmd.data.z, 20);     EXPECT_EQ(cmd.usedFields._z, 1);
    EXPECT_FLOAT_EQ(cmd.data.e, 5);      EXPECT_EQ(cmd.usedFields._e, 1);
    EXPECT_FLOAT_EQ(cmd.data.f, 10);     EXPECT_EQ(cmd.usedFields._f, 1);
    EXPECT_FLOAT_EQ(cmd.data.s, 0);      EXPECT_EQ(cmd.usedFields._s, 0);

    EXPECT_CALL(*mock, motorSetMove(_, _, _)).WillRepeatedly(Return(false));
    EXPECT_CALL(*mock, motorStart(_)).WillRepeatedly(Return(false));
    EXPECT_CALL(*mock, motorIsOn(_)).WillRepeatedly(Return(false));
    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    //EXPECT_EQ(settings->motors[MOTOR_X].data.position, 0);
    //EXPECT_EQ(settings->motors[MOTOR_Y].data.position, 0);
    //EXPECT_EQ(settings->motors[MOTOR_Z1].data.position, 0);
    //EXPECT_EQ(settings->motors[MOTOR_Z2].data.position, 0);
}

TEST_F(ParserGCode_test, ParserGCode__command_G28__test)
{
    /*TODO: make this test better*/
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "G28";


    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.data.x, 0);     EXPECT_EQ(cmd.usedFields._x, 0);
    EXPECT_FLOAT_EQ(cmd.data.y, 0);     EXPECT_EQ(cmd.usedFields._y, 0);
    EXPECT_FLOAT_EQ(cmd.data.z, 0);     EXPECT_EQ(cmd.usedFields._z, 0);
    EXPECT_FLOAT_EQ(cmd.data.e, 0);     EXPECT_EQ(cmd.usedFields._e, 0);
    EXPECT_FLOAT_EQ(cmd.data.f, 0);     EXPECT_EQ(cmd.usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd.data.s, 0);     EXPECT_EQ(cmd.usedFields._s, 0);

    EXPECT_CALL(*mock, motorSetMove(_, _, _)).WillRepeatedly(Return(false));
    EXPECT_CALL(*mock, motorStart(_)).WillRepeatedly(Return(false));
    EXPECT_CALL(*mock, motorIsOn(_)).WillRepeatedly(Return(false));
    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    //EXPECT_EQ(settings->motors[MOTOR_X].data.position, 0);
    //EXPECT_EQ(settings->motors[MOTOR_Y].data.position, 0);
    //EXPECT_EQ(settings->motors[MOTOR_Z1].data.position, 0);
    //EXPECT_EQ(settings->motors[MOTOR_Z2].data.position, 0);
}

TEST_F(ParserGCode_test, ParserGCode__command_G90__test)
{
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "G90";


    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.data.x, 0);     EXPECT_EQ(cmd.usedFields._x, 0);
    EXPECT_FLOAT_EQ(cmd.data.y, 0);     EXPECT_EQ(cmd.usedFields._y, 0);
    EXPECT_FLOAT_EQ(cmd.data.z, 0);     EXPECT_EQ(cmd.usedFields._z, 0);
    EXPECT_FLOAT_EQ(cmd.data.e, 0);     EXPECT_EQ(cmd.usedFields._e, 0);
    EXPECT_FLOAT_EQ(cmd.data.f, 0);     EXPECT_EQ(cmd.usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd.data.s, 0);     EXPECT_EQ(cmd.usedFields._s, 0);

    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(settings->positioningMode, ABSOLUTE);
}

TEST_F(ParserGCode_test, ParserGCode__command_G91__test)
{
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "G91";


    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.data.x, 0);     EXPECT_EQ(cmd.usedFields._x, 0);
    EXPECT_FLOAT_EQ(cmd.data.y, 0);     EXPECT_EQ(cmd.usedFields._y, 0);
    EXPECT_FLOAT_EQ(cmd.data.z, 0);     EXPECT_EQ(cmd.usedFields._z, 0);
    EXPECT_FLOAT_EQ(cmd.data.e, 0);     EXPECT_EQ(cmd.usedFields._e, 0);
    EXPECT_FLOAT_EQ(cmd.data.f, 0);     EXPECT_EQ(cmd.usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd.data.s, 0);     EXPECT_EQ(cmd.usedFields._s, 0);

    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(settings->positioningMode, RELATIVE);
}

TEST_F(ParserGCode_test, ParserGCode__command_G92__test)
{
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "G92 X10 Y20 Z30 E15";


    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.data.x, 10);    EXPECT_EQ(cmd.usedFields._x, 1);
    EXPECT_FLOAT_EQ(cmd.data.y, 20);    EXPECT_EQ(cmd.usedFields._y, 1);
    EXPECT_FLOAT_EQ(cmd.data.z, 30);    EXPECT_EQ(cmd.usedFields._z, 1);
    EXPECT_FLOAT_EQ(cmd.data.e, 15);    EXPECT_EQ(cmd.usedFields._e, 1);
    EXPECT_FLOAT_EQ(cmd.data.f, 0);     EXPECT_EQ(cmd.usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd.data.s, 0);     EXPECT_EQ(cmd.usedFields._s, 0);

    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(settings->motors[MOTOR_X]->data.position, 10 * ACCURACY);
    EXPECT_EQ(settings->motors[MOTOR_Y]->data.position, 20 * ACCURACY);
    EXPECT_EQ(settings->motors[MOTOR_Z1]->data.position, 30 * ACCURACY);
    EXPECT_EQ(settings->motors[MOTOR_Z2]->data.position, 30 * ACCURACY);
    //EXPECT_EQ(settings->motors[MOTOR_E]->data.position, 15 * ACCURACY);
}

TEST_F(ParserGCode_test, ParserGCode__command_M104__test)
{
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "M104 S80";

    std::cout << "[WARNING] Not Implemented" << std::endl;


    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.data.x, 0);     EXPECT_EQ(cmd.usedFields._x, 0);
    EXPECT_FLOAT_EQ(cmd.data.y, 0);     EXPECT_EQ(cmd.usedFields._y, 0);
    EXPECT_FLOAT_EQ(cmd.data.z, 0);     EXPECT_EQ(cmd.usedFields._z, 0);
    EXPECT_FLOAT_EQ(cmd.data.e, 0);     EXPECT_EQ(cmd.usedFields._e, 0);
    EXPECT_FLOAT_EQ(cmd.data.f, 0);     EXPECT_EQ(cmd.usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd.data.s, 80);    EXPECT_EQ(cmd.usedFields._s, 1);

    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
}

TEST_F(ParserGCode_test, ParserGCode__command_M109__test)
{
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "M109 S80";

    std::cout << "[WARNING] Not Implemented" << std::endl;


    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.data.x, 0);     EXPECT_EQ(cmd.usedFields._x, 0);
    EXPECT_FLOAT_EQ(cmd.data.y, 0);     EXPECT_EQ(cmd.usedFields._y, 0);
    EXPECT_FLOAT_EQ(cmd.data.z, 0);     EXPECT_EQ(cmd.usedFields._z, 0);
    EXPECT_FLOAT_EQ(cmd.data.e, 0);     EXPECT_EQ(cmd.usedFields._e, 0);
    EXPECT_FLOAT_EQ(cmd.data.f, 0);     EXPECT_EQ(cmd.usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd.data.s, 80);    EXPECT_EQ(cmd.usedFields._s, 1);

    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
}

TEST_F(ParserGCode_test, ParserGCode__command_M140__test)
{
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "M140 S80";

    std::cout << "[WARNING] Not Implemented" << std::endl;


    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.data.x, 0);     EXPECT_EQ(cmd.usedFields._x, 0);
    EXPECT_FLOAT_EQ(cmd.data.y, 0);     EXPECT_EQ(cmd.usedFields._y, 0);
    EXPECT_FLOAT_EQ(cmd.data.z, 0);     EXPECT_EQ(cmd.usedFields._z, 0);
    EXPECT_FLOAT_EQ(cmd.data.e, 0);     EXPECT_EQ(cmd.usedFields._e, 0);
    EXPECT_FLOAT_EQ(cmd.data.f, 0);     EXPECT_EQ(cmd.usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd.data.s, 80);    EXPECT_EQ(cmd.usedFields._s, 1);

    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
}

TEST_F(ParserGCode_test, ParserGCode__command_M190__test)
{
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "M190 S80";

    std::cout << "[WARNING] Not Implemented" << std::endl;


    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.data.x, 0);     EXPECT_EQ(cmd.usedFields._x, 0);
    EXPECT_FLOAT_EQ(cmd.data.y, 0);     EXPECT_EQ(cmd.usedFields._y, 0);
    EXPECT_FLOAT_EQ(cmd.data.z, 0);     EXPECT_EQ(cmd.usedFields._z, 0);
    EXPECT_FLOAT_EQ(cmd.data.e, 0);     EXPECT_EQ(cmd.usedFields._e, 0);
    EXPECT_FLOAT_EQ(cmd.data.f, 0);     EXPECT_EQ(cmd.usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd.data.s, 80);    EXPECT_EQ(cmd.usedFields._s, 1);

    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
}

TEST_F(ParserGCode_test, ParserGCode__command_M106__test)
{
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "M106 S100";

    std::cout << "[WARNING] Not Implemented" << std::endl;


    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FLOAT_EQ(cmd.data.x, 0);     EXPECT_EQ(cmd.usedFields._x, 0);
    EXPECT_FLOAT_EQ(cmd.data.y, 0);     EXPECT_EQ(cmd.usedFields._y, 0);
    EXPECT_FLOAT_EQ(cmd.data.z, 0);     EXPECT_EQ(cmd.usedFields._z, 0);
    EXPECT_FLOAT_EQ(cmd.data.e, 0);     EXPECT_EQ(cmd.usedFields._e, 0);
    EXPECT_FLOAT_EQ(cmd.data.f, 0);     EXPECT_EQ(cmd.usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd.data.s, 100);   EXPECT_EQ(cmd.usedFields._s, 1);

    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);
}

TEST_F(ParserGCode_test, ParserGCode__WrongCommand__test)
{
    Std_Err stdErr = STD_OK;
    char* sendCmd;

    GCodeCommand cmd;
    char cmdStr[] = "AAA BBB CCC DDD";

    stdErr = parseGCodeCommand(cmdStr, &cmd);
    EXPECT_EQ(stdErr, STD_ERROR);

    EXPECT_FLOAT_EQ(cmd.data.x, 0);
    EXPECT_FLOAT_EQ(cmd.data.y, 0);
    EXPECT_FLOAT_EQ(cmd.data.z, 0);
    EXPECT_FLOAT_EQ(cmd.data.e, 0);
    EXPECT_FLOAT_EQ(cmd.data.f, 0);
    EXPECT_FLOAT_EQ(cmd.data.s, 0);

    stdErr = executeGCodeCommand(&cmd, settings);
    EXPECT_EQ(stdErr, STD_PARAMETER_ERROR);
}



/************************** PUBLIC FUNCTIONS **************************/

void ParserGCode_test::setupMotor(MotorSettings* motor)
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

void ParserGCode_test::setupOuterComm(OuterComm_Settings* settings)
{
	settings->Buff_InputCommands = NULL;
	settings->Buff_IN = NULL;
	settings->Buff_OUT = NULL;
	settings->huart = &huart1;
	settings->EOL_recieved = false;
	settings->transmission = false;
}

void ParserGCode_test::setupEEPROM(EEPROMSettings* eeprom)
{
	eeprom->isReady = true;
    eeprom->i2c = &hi2c1;
}

void ParserGCode_test::setupSD(SDCard_Settings* sd)
{
	sd->end_SDprogram = false;
    sd->executing_SDcommand = false;
    sd->executing_SDprogram = false;
}

void ParserGCode_test::setupDevice(DeviceSettings* settings)
{
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        setupMotor(settings->motors[i]);
    }

    setupOuterComm(settings->outComm);
    setupEEPROM(settings->eeprom);
    setupSD(settings->sd);
}



/************************** MOCK DEFINITIONS **************************/

extern "C"
{
    void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
    {
        ParserGCode_test::mock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
    }

    bool motorSetMove(MotorSettings* settings, double move, RoundingErrorData* roundingError)
    {
        ParserGCode_test::mock->motorSetMove(settings, move, roundingError);
    }

    bool motorStart(MotorSettings* settings)
    {
        ParserGCode_test::mock->motorStart(settings);
    }

    bool motorIsOn(MotorSettings* settings)
    {
        ParserGCode_test::mock->motorIsOn(settings);
    }
}


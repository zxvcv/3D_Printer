#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdint.h>
#include <string.h>

#define ACCURACY 1000
#define BYTES_TO_READ 50

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArrayArgument;
using ::testing::SetArgPointee;


extern "C"
{
    #include "managerSDCard.h"
    #include "ProjectObjects.h"
    #include "BT.h"
    #include "parserGCode.h"
}

enum {
		RELATIVE,
		ABSOLUTE
}positioningMode;

class Mock_managerSDCard {
public:
    MOCK_METHOD3(HAL_UART_Receive_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size));
    MOCK_METHOD3(HAL_UART_Transmit_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size));
    MOCK_METHOD4(f_read, FRESULT(FIL* fp, void* buff, UINT btr, UINT* br));
    MOCK_METHOD1(f_close, FRESULT(FIL* fp));
    MOCK_METHOD3(HAL_GPIO_WritePin, void(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState));
};

class ManagerSDCard_test : public ::testing::Test {
public:
    ManagerSDCard_test()
    {
        mock = new Mock_managerSDCard();
    }
    ~ManagerSDCard_test()
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
        settings->sd = (SDCard_Settings*)malloc(sizeof(SDCard_Settings));
        
        setupDevice(settings);
        EXPECT_CALL(*mock, HAL_UART_Receive_IT(settings->bt->huart, _, _))
                .WillOnce(Return(HAL_OK));
        stdErr = init_operations_BT(settings->bt);
        EXPECT_EQ(stdErr, STD_OK);

        stdErr = init_operations_SDcard(settings->sd);
        EXPECT_EQ(stdErr, STD_OK);
    }

    virtual void TearDown()
    {
        Std_Err stdErr;
        Fifo_Err fifoErr;

        stdErr = deinit_operations_BT(settings->bt);
        EXPECT_EQ(stdErr, STD_OK);

        list_delete_C(&(settings->sd->BuffIN_SDcmd), &fifoErr);
        EXPECT_EQ(fifoErr, QUEUE_OK);

        free(settings->sd);
        free(settings->bt);
        for(int i=0; i<MOTORS_NUM; ++i)
        {
            free(settings->motors[i]);
        }
        free(settings);
    }


    void setupMotor(MotorSettings* motor);
    void setupBT(BT_Settings* bt);
    void setupSDCard(SDCard_Settings* sd);
    void setupDevice(DeviceSettings* settings);

    void checkGCommandValues(GCodeCommand* cmd, uint8_t abs_rel);
    void checkGCommandParserValues(uint8_t abs_rel, uint8_t listCorrectSize);
    void checkGCommandExecuteValues(uint8_t abs_rel, uint8_t listCorrectSize);


    DeviceSettings* settings;

    static Mock_managerSDCard* mock;
};

Mock_managerSDCard* ManagerSDCard_test::mock;



/************************** TESTS **************************/

TEST_F(ManagerSDCard_test, ManagerSDCard__parse_data_SDcard_inOneBuff__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;

    GCodeCommand* cmd;
    uint8_t listSize;
    char cmdStr[26] = "G91\r\nG90\r\nG91\r\nG90\r\nG91\r\n";

    settings->sd->executing_SDprogram = true;

    EXPECT_FALSE(settings->sd->end_SDprogram);
    EXPECT_TRUE(settings->sd->executing_SDprogram);
    EXPECT_EQ(settings->sd->counterTab[0], BYTES_TO_READ);
    EXPECT_EQ(settings->sd->counterTab[1], BYTES_TO_READ);

    /*below for should be done in function parse_data_SDcard, but SetArrayArgument dont work with (void*) */
    for(int i=0; i<25; ++i)
    {
        settings->sd->dataSDin[0][i] = cmdStr[i];
    }

    EXPECT_CALL(*mock, f_read(settings->sd->file, _, BYTES_TO_READ, _))
        .WillOnce(DoAll(
            //SetArrayArgument<1>(cmdStr, cmdStr + 21),
            SetArgPointee<3>(25),
            Return(FR_OK)
        ));
    stdErr = parse_data_SDcard(settings->sd);
    EXPECT_EQ(stdErr, STD_OK);

    listSize = list_getSize(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    EXPECT_EQ(listSize, 5);

    /*1st cmd G91*/
    cmd = (GCodeCommand*)list_front(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    
    checkGCommandValues(cmd, RELATIVE);
    
    list_pop_C(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);

    /*2nd cmd G90*/
    cmd = (GCodeCommand*)list_front(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    
    checkGCommandValues(cmd, ABSOLUTE);
    
    list_pop_C(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);

    /*3td cmd G91*/
    cmd = (GCodeCommand*)list_front(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    
    checkGCommandValues(cmd, RELATIVE);
    
    list_pop_C(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);

    /*4th cmd G90*/
    cmd = (GCodeCommand*)list_front(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    
    checkGCommandValues(cmd, ABSOLUTE);
    
    list_pop_C(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);

    /*5th cmd G91*/
    cmd = (GCodeCommand*)list_front(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    
    checkGCommandValues(cmd, RELATIVE);
    
    list_pop_C(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);

    /*end*/
    listSize = list_getSize(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    EXPECT_EQ(listSize, 0);
}

TEST_F(ManagerSDCard_test, ManagerSDCard__parse_data_SDcard_longWorkTest__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;

    uint8_t listSize;
    char cmdStr[] = 
    "G91\r\nG90\r\nG91\r\nG90\r\nG91\r\nG90\r\nG91\r\nG90\r\nG91\r\nG90\r\nG91\r\nG90\r\nG91\r\n";

    settings->sd->executing_SDprogram = true;

    EXPECT_FALSE(settings->sd->end_SDprogram);
    EXPECT_TRUE(settings->sd->executing_SDprogram);
    EXPECT_EQ(settings->sd->activeTab, 1);
    EXPECT_EQ(settings->sd->cnt, 0);
    EXPECT_EQ(settings->sd->counterTab[0], BYTES_TO_READ);
    EXPECT_EQ(settings->sd->counterTab[1], BYTES_TO_READ);

    /*below for should be done in function parse_data_SDcard, but SetArrayArgument dont work with (void*) */
    for(int i=0; i<69; ++i)
    {
        if(i < 50)
        {
            settings->sd->dataSDin[0][i] = cmdStr[i];
        }
        else
        {
            settings->sd->dataSDin[1][i%50] = cmdStr[i];
        }
    }

    EXPECT_CALL(*mock, f_read(settings->sd->file, _, BYTES_TO_READ, _))
        .WillOnce(DoAll(
            SetArgPointee<3>(50),
            Return(FR_OK)
        ));
    stdErr = parse_data_SDcard(settings->sd);
    EXPECT_EQ(stdErr, STD_OK);

    listSize = list_getSize(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    EXPECT_EQ(listSize, 5);

    /*1st cmd G91*/
    checkGCommandParserValues(RELATIVE, 5);

    /*2nd cmd G90*/
    checkGCommandParserValues(ABSOLUTE, 5);

    /*3td cmd G91*/
    checkGCommandParserValues(RELATIVE, 5);

    /*4th cmd G90*/
    checkGCommandParserValues(ABSOLUTE, 5);
  
    /*5th cmd G91*/
    checkGCommandParserValues(RELATIVE, 5);

    /*6th cmd G90*/
    EXPECT_CALL(*mock, f_read(settings->sd->file, _, BYTES_TO_READ, _))
        .WillOnce(DoAll(
            SetArgPointee<3>(15),
            Return(FR_OK)
        ));
    checkGCommandParserValues(ABSOLUTE, 5);
 
    /*7th cmd G91*/
    checkGCommandParserValues(RELATIVE, 5);

    /*8th cmd G90*/
    checkGCommandParserValues(ABSOLUTE, 5);
    
    /*9th cmd G91*/
    checkGCommandParserValues(RELATIVE, 4);

    /*10th cmd G90*/
    checkGCommandParserValues(ABSOLUTE, 3);

    /*11th cmd G91*/
    checkGCommandParserValues(RELATIVE, 2);

    /*12th cmd G90*/
    checkGCommandParserValues(ABSOLUTE, 1);

    /*13th cmd G91*/
    checkGCommandParserValues(RELATIVE, 0);
    
    /*end*/
    EXPECT_TRUE(settings->sd->eofRecieved);
    EXPECT_TRUE(settings->sd->end_SDprogram);
    EXPECT_TRUE(settings->sd->executing_SDprogram);
    EXPECT_EQ(settings->sd->activeTab, 1);
}
    
TEST_F(ManagerSDCard_test, ManagerSDCard__execute_command_SDcard_longWorkTest__test)
{
    Std_Err stdErr = STD_OK;
    Fifo_Err fifoErr;

    uint8_t listSize;
    char cmdStr[] = 
    "G91\r\nG90\r\nG91\r\nG90\r\nG91\r\nG90\r\nG91\r\nG90\r\nG91\r\nG90\r\nG91\r\nG90\r\nG91\r\n";

    settings->sd->executing_SDprogram = true;

    EXPECT_FALSE(settings->sd->end_SDprogram);
    EXPECT_TRUE(settings->sd->executing_SDprogram);
    EXPECT_EQ(settings->sd->activeTab, 1);
    EXPECT_EQ(settings->sd->cnt, 0);
    EXPECT_EQ(settings->sd->counterTab[0], BYTES_TO_READ);
    EXPECT_EQ(settings->sd->counterTab[1], BYTES_TO_READ);

    settings->sd->activeTab = 0;
    settings->sd->unactiveTab = 1;

    /*below for should be done in function parse_data_SDcard, but SetArrayArgument dont work with (void*) */
    for(int i=0; i<69; ++i)
    {
        if(i < 50)
        {
            settings->sd->dataSDin[1][i] = cmdStr[i];
        }
        else
        {
            settings->sd->dataSDin[0][i%50] = cmdStr[i];
        }
    }

    EXPECT_CALL(*mock, f_read(settings->sd->file, _, BYTES_TO_READ, _))
        .WillOnce(DoAll(
            SetArgPointee<3>(50),
            Return(FR_OK)
        ));
    stdErr = parse_data_SDcard(settings->sd);
    EXPECT_EQ(stdErr, STD_OK);

    listSize = list_getSize(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    EXPECT_EQ(listSize, 5);

    /*1st cmd G91*/
    
    checkGCommandExecuteValues(RELATIVE, 5);

    /*2nd cmd G90*/
    checkGCommandExecuteValues(ABSOLUTE, 5);

    /*3td cmd G91*/
    checkGCommandExecuteValues(RELATIVE, 5);

    /*4th cmd G90*/
    checkGCommandExecuteValues(ABSOLUTE, 5);
  
    /*5th cmd G91*/
    checkGCommandExecuteValues(RELATIVE, 5);

    /*6th cmd G90*/
    EXPECT_CALL(*mock, f_read(settings->sd->file, _, BYTES_TO_READ, _))
        .WillOnce(DoAll(
            SetArgPointee<3>(15),
            Return(FR_OK)
        ));
    checkGCommandExecuteValues(ABSOLUTE, 5);
 
    /*7th cmd G91*/
    checkGCommandExecuteValues(RELATIVE, 5);

    /*8th cmd G90*/
    checkGCommandExecuteValues(ABSOLUTE, 5);
    
    /*9th cmd G91*/
    checkGCommandExecuteValues(RELATIVE, 4);

    /*10th cmd G90*/
    checkGCommandExecuteValues(ABSOLUTE, 3);

    /*11th cmd G91*/
    checkGCommandExecuteValues(RELATIVE, 2);

    /*12th cmd G90*/
    checkGCommandExecuteValues(ABSOLUTE, 1);

    /*13th cmd G91*/
    checkGCommandExecuteValues(RELATIVE, 0);
    
    /*end*/
    EXPECT_TRUE(settings->sd->eofRecieved);
    EXPECT_TRUE(settings->sd->end_SDprogram);
    EXPECT_TRUE(settings->sd->executing_SDprogram);
    EXPECT_EQ(settings->sd->activeTab, 0);
}

TEST_F(ManagerSDCard_test, ManagerSDCard__reset_commands_SDcard__test)
{
    Std_Err stdErr;
    Fifo_Err fifoErr;
    uint8_t listSize;

    settings->sd->end_SDprogram = true;
    settings->sd->executing_SDprogram = true;
    settings->sd->executing_SDcommand = false;
    
    listSize = list_getSize(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(listSize, 0);

    EXPECT_CALL(*mock, f_close(settings->sd->file));
    stdErr = reset_commands_SDcard(settings->sd);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FALSE(settings->sd->end_SDprogram);
    EXPECT_FALSE(settings->sd->executing_SDprogram);
}

TEST_F(ManagerSDCard_test, ManagerSDCard__detecting_endCommand_SDcard_longWorkTest__test)
{
    Std_Err stdErr;
    uint8_t listSize;

    settings->sd->executing_SDcommand = true;


    stdErr = detecting_endCommand_SDcard(settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_FALSE(settings->sd->executing_SDcommand);

    stdErr = detecting_endCommand_SDcard(settings);
    EXPECT_EQ(stdErr, STD_BUSY_ERROR);
}



/************************** PUBLIC FUNCTIONS **************************/

void ManagerSDCard_test::setupMotor(MotorSettings* motor)
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

void ManagerSDCard_test::setupBT(BT_Settings* bt)
{
	bt->Buff_InputCommandsBT = NULL;
	bt->Buff_Bt_IN = NULL;
	bt->Buff_Bt_OUT = NULL;
	bt->huart = &huart1;
	bt->EOL_BT_recieved = false;
	bt->transmissionBT = false;
}

void ManagerSDCard_test::setupSDCard(SDCard_Settings* sd)
{
	sd->file = NULL;
    sd->BuffIN_SDcmd = NULL;

    memset(sd->dataSDin[0], 0, BYTES_TO_READ);
    memset(sd->dataSDin[1], 0, BYTES_TO_READ);

#ifdef LOG_ENABLE
	sd->logFile = NULL;
	sd->BuffOUT_logs = NULL;
#endif /*LOG_ENABLE*/
}

void ManagerSDCard_test::setupDevice(DeviceSettings* settings)
{
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        setupMotor(settings->motors[i]);
    }

    setupBT(settings->bt);
    setupSDCard(settings->sd);

    settings->errMove = false;
}


void ManagerSDCard_test::checkGCommandValues(GCodeCommand* cmd, uint8_t abs_rel)
{
    Std_Err stdErr;

    EXPECT_FLOAT_EQ(cmd->data.x, 0);     EXPECT_EQ(cmd->usedFields._x, 0);
    EXPECT_FLOAT_EQ(cmd->data.y, 0);     EXPECT_EQ(cmd->usedFields._y, 0);
    EXPECT_FLOAT_EQ(cmd->data.z, 0);     EXPECT_EQ(cmd->usedFields._z, 0);
    EXPECT_FLOAT_EQ(cmd->data.e, 0);     EXPECT_EQ(cmd->usedFields._e, 0);
    EXPECT_FLOAT_EQ(cmd->data.f, 0);     EXPECT_EQ(cmd->usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd->data.s, 0);     EXPECT_EQ(cmd->usedFields._s, 0);

    stdErr = executeGCodeCommand(cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(settings->positioningMode, abs_rel);
}

void ManagerSDCard_test::checkGCommandParserValues(uint8_t abs_rel, uint8_t listCorrectSize)
{
    Std_Err stdErr;
    Fifo_Err fifoErr;
    uint8_t listSize;
    GCodeCommand* cmd;

    cmd = (GCodeCommand*)list_front(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);

    EXPECT_FLOAT_EQ(cmd->data.x, 0);     EXPECT_EQ(cmd->usedFields._x, 0);
    EXPECT_FLOAT_EQ(cmd->data.y, 0);     EXPECT_EQ(cmd->usedFields._y, 0);
    EXPECT_FLOAT_EQ(cmd->data.z, 0);     EXPECT_EQ(cmd->usedFields._z, 0);
    EXPECT_FLOAT_EQ(cmd->data.e, 0);     EXPECT_EQ(cmd->usedFields._e, 0);
    EXPECT_FLOAT_EQ(cmd->data.f, 0);     EXPECT_EQ(cmd->usedFields._f, 0);
    EXPECT_FLOAT_EQ(cmd->data.s, 0);     EXPECT_EQ(cmd->usedFields._s, 0);

    stdErr = executeGCodeCommand(cmd, settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(settings->positioningMode, abs_rel);
    
    list_pop_C(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);

    stdErr = parse_data_SDcard(settings->sd);
    EXPECT_EQ(stdErr, STD_OK);

    listSize = list_getSize(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    EXPECT_EQ(listSize, listCorrectSize);
}

void ManagerSDCard_test::checkGCommandExecuteValues(uint8_t abs_rel, uint8_t listCorrectSize)
{
    Std_Err stdErr;
    Fifo_Err fifoErr;
    uint8_t listSize;

    stdErr = execute_command_SDcard(settings);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_EQ(settings->positioningMode, abs_rel);

    stdErr = parse_data_SDcard(settings->sd);
    EXPECT_EQ(stdErr, STD_OK);

    listSize = list_getSize(settings->sd->BuffIN_SDcmd, &fifoErr);
    EXPECT_EQ(fifoErr, QUEUE_OK);
    EXPECT_EQ(listSize, listCorrectSize);
} 



extern "C"
{
    HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        ManagerSDCard_test::mock->HAL_UART_Receive_IT(huart, pData, Size);
    }

    HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        ManagerSDCard_test::mock->HAL_UART_Transmit_IT(huart, pData, Size);
    }

    FRESULT f_read(FIL* fp, void* buff, UINT btr, UINT* br)
    {
        ManagerSDCard_test::mock->f_read(fp, buff, btr, br);
    }

    FRESULT f_close(FIL* fp)
    {
        ManagerSDCard_test::mock->f_close(fp);
    }

    void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
    {
        ManagerSDCard_test::mock->HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
    }
}


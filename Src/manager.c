/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 *
 * =======================================================================================================
 * COMMENTS:
 *
 * =======================================================================================================
 * EXAMPLE:
 *
 ********************************************************************************************************** */



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include "manager.h"
#include "main.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

//#define USE_INTERRUPTS
//#define LOG_ENABLE



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PRIVATE OBJECTS
 * ####################################################################################################### */

MotorSettings motors[MOTORS_NUM] = {
		{
				.IOreset = { .PORT = MOT1_RESET_GPIO_Port, .PIN = MOT1_RESET_Pin },
				.IOsleep = { .PORT = MOT1_SLEEP_GPIO_Port, .PIN = MOT1_SLEEP_Pin },
				.IOdirection = { .PORT = MOT1_DIRECTION_GPIO_Port, .PIN = MOT1_DIRECTION_Pin },
				.IOstep = { .PORT = MOT1_STEP_GPIO_Port, .PIN = MOT1_STEP_Pin },
				//.IOstep = { .PORT = LD2_GPIO_Port, .PIN = LD2_Pin },
				.isReversed = false,
				.timerFrequency = 1000,
				.stepSize = 203,
				.data.motorNum = 1,
				.eepromDataAddress = 0x00
		},
		{
				.IOreset = { .PORT = MOT2_RESET_GPIO_Port, .PIN = MOT2_RESET_Pin },
				.IOsleep = { .PORT = MOT2_SLEEP_GPIO_Port, .PIN = MOT2_SLEEP_Pin },
				.IOdirection = { .PORT = MOT2_DIRECTION_GPIO_Port, .PIN = MOT2_DIRECTION_Pin },
				.IOstep = { .PORT = MOT2_STEP_GPIO_Port, .PIN = MOT2_STEP_Pin },
				.isReversed = false,
				.timerFrequency = 1000,
				.stepSize = 203,
				.data.motorNum = 2,
				.eepromDataAddress = 0x00 + sizeof(MotorData_EEPROM)
		},
		{
				.IOreset = { .PORT = MOT3_RESET_GPIO_Port, .PIN = MOT3_RESET_Pin },
				.IOsleep = { .PORT = MOT3_SLEEP_GPIO_Port, .PIN = MOT3_SLEEP_Pin },
				.IOdirection = { .PORT = MOT3_DIRECTION_GPIO_Port, .PIN = MOT3_DIRECTION_Pin },
				.IOstep = { .PORT = MOT3_STEP_GPIO_Port, .PIN = MOT3_STEP_Pin },
				.isReversed = false,
				.timerFrequency = 1000,
				.stepSize = 203,
				.data.motorNum = 3,
				.eepromDataAddress = 0x00 + 2*sizeof(MotorData_EEPROM)
		},
		{
				.IOreset = { .PORT = MOT4_RESET_GPIO_Port, .PIN = MOT4_RESET_Pin },
				.IOsleep = { .PORT = MOT4_SLEEP_GPIO_Port, .PIN = MOT4_SLEEP_Pin },
				.IOdirection = { .PORT = MOT4_DIRECTION_GPIO_Port, .PIN = MOT4_DIRECTION_Pin },
				.IOstep = { .PORT = MOT4_STEP_GPIO_Port, .PIN = MOT4_STEP_Pin },
				.isReversed = false,
				.timerFrequency = 1000,
				.stepSize = 203,
				.data.motorNum = 4,
				.eepromDataAddress = 0x00 + 2*sizeof(MotorData_EEPROM)
		}
};

EEPROMSettings eeprom = {
		.isReady = true,
		.i2c = &hi2c1
};

ST7565R_Settings lcd = { .IOcs = { .PORT = ST7565R_CS_GPIO_Port, .PIN = ST7565R_CS_Pin },
						 .IOrst = { .PORT = ST7565R_RST_GPIO_Port, .PIN = ST7565R_RST_Pin },
						 .IOa0 = { .PORT = ST7565R_A0_GPIO_Port, .PIN = ST7565R_A0_Pin },
						 .spi = &hspi2,
						 .buffer = NULL
};

BT_Settings bt = {
		.Buff_InputCommandsBT = NULL,
		.Buff_Bt_IN = NULL,
		.Buff_Bt_OUT = NULL,
		.EOL_BT_recieved = false,
		.transmissionBT = false
};

FATFS fatfs;

FIL file;

#ifdef LOG_ENABLE
FIL logFile;
#endif /*LOG_ENABLE*/

SDCard_Settings sd = {
		.file = &file,
		.BuffIN_SDcmd = NULL,
		.end_SDprogram = false,
		.executing_SDprogram = false,
		.executing_SDcommand = false,
#ifdef LOG_ENABLE
		.logFile = &logFile,
		.BuffOUT_logs = NULL
#endif /*LOG_ENABLE*/
};

/* #######################################################################################################
 *										PUBLIC OBJECTS
 * ####################################################################################################### */

DeviceSettings printerSettings = {
		.positioningMode = ABSOLUTE,
		.sdCommandState = IDLE,
		.fatfs = &fatfs,
		.sd = &sd,
		.motors = motors,
		.eeprom = &eeprom,
		.lcd = &lcd,
		.bt = &bt,
		.errMove = false,
		.speed = 0.0,
		.recievedBT = 0,
		.EOL_BT_recieved = false,
		.transmissionBT = false
};



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

void init_manager(DeviceSettings* settings){
	init_operations_BT(settings->bt);

	//Initialize SD Card
	HAL_GPIO_WritePin(SDSPI_CS_GPIO_Port, SDSPI_CS_Pin, GPIO_PIN_SET);

	init_operations_SDcard(settings->sd);
}

void clearAllMotorsRoundingErrors(DeviceSettings *printerSettings){
	motors[0].data.err.roundingMoveError = 0.0;
	motors[0].data.err.roundingSpeedError = 0.0;
	motors[0].data.err.errMove = false;
	motors[1].data.err.roundingMoveError = 0.0;
	motors[1].data.err.roundingSpeedError = 0.0;
	motors[1].data.err.errMove = false;
	motors[2].data.err.roundingMoveError = 0.0;
	motors[2].data.err.roundingSpeedError = 0.0;
	motors[2].data.err.errMove = false;
	motors[3].data.err.roundingMoveError = 0.0;
	motors[3].data.err.roundingSpeedError = 0.0;
	motors[3].data.err.errMove = false;
}

void getMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns){
	MotorData_EEPROM data;
	EEPROM_readData(memSettigns, motSettings->eepromDataAddress, (uint8_t*)(&data), sizeof(MotorData_EEPROM));
	motSettings->stepSize = data.stepSize;
	motSettings->data.maxSpeed = data.maxSpeed;
	motSettings->data.positionZero = data.positionZero;
	motSettings->data.positionEnd = data.positionEnd;
}

void setMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns, MotorData_EEPROM *data){
	EEPROM_writeData(memSettigns, motSettings->eepromDataAddress, (uint8_t*)data, sizeof(MotorData_EEPROM));
}

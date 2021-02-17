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

#include "ProjectObjects.h"
#include "main.h"


/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

#define ACCURACY 1000



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart2;



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PRIVATE OBJECTS
 * ####################################################################################################### */

MotorSettings motor1 = {
		.IOreset = { .PORT = MOT1_RESET_GPIO_Port, .PIN = MOT1_RESET_Pin },
		.IOsleep = { .PORT = MOT1_SLEEP_GPIO_Port, .PIN = MOT1_SLEEP_Pin },
		.IOdirection = { .PORT = MOT1_DIRECTION_GPIO_Port, .PIN = MOT1_DIRECTION_Pin },
		.IOstep = { .PORT = MOT1_STEP_GPIO_Port, .PIN = MOT1_STEP_Pin },
		//.IOstep = { .PORT = LD2_GPIO_Port, .PIN = LD2_Pin },

		.flags = {
				.isOn = 0,
				.reset = 0,
				.sleep = 0,
				.stepPhase = 0,
				.direction = 0,
		},

		.device = {
				.motorNum = 1,
				.eepromDataAddress = 0x00,
				.timerFrequency = 1000,
				.stepSize = 203,
				.isReversed = false,
				.maxSpeed = 50,
				.positionZero = 0 * ACCURACY,
				.positionEnd = 20 * ACCURACY,
		},
};

MotorSettings motor2 = {
		.IOreset = { .PORT = MOT2_RESET_GPIO_Port, .PIN = MOT2_RESET_Pin },
		.IOsleep = { .PORT = MOT2_SLEEP_GPIO_Port, .PIN = MOT2_SLEEP_Pin },
		.IOdirection = { .PORT = MOT2_DIRECTION_GPIO_Port, .PIN = MOT2_DIRECTION_Pin },
		.IOstep = { .PORT = MOT2_STEP_GPIO_Port, .PIN = MOT2_STEP_Pin },

		.flags = {
				.isOn = 0,
				.reset = 0,
				.sleep = 0,
				.stepPhase = 0,
				.direction = 0,
		},

		.device = {
				.motorNum = 2,
				.eepromDataAddress = 0x00 + sizeof(MotorData_EEPROM),
				.timerFrequency = 1000,
				.stepSize = 203,
				.isReversed = false,
				.maxSpeed = 50,
				.positionZero = 0 * ACCURACY,
				.positionEnd = 20 * ACCURACY,
		},
};

MotorSettings motor3 = {
		.IOreset = { .PORT = MOT3_RESET_GPIO_Port, .PIN = MOT3_RESET_Pin },
		.IOsleep = { .PORT = MOT3_SLEEP_GPIO_Port, .PIN = MOT3_SLEEP_Pin },
		.IOdirection = { .PORT = MOT3_DIRECTION_GPIO_Port, .PIN = MOT3_DIRECTION_Pin },
		.IOstep = { .PORT = MOT3_STEP_GPIO_Port, .PIN = MOT3_STEP_Pin },

		.flags = {
				.isOn = 0,
				.reset = 0,
				.sleep = 0,
				.stepPhase = 0,
				.direction = 0,
		},

		.device = {
				.motorNum = 3,
				.eepromDataAddress = 0x00 + 2*sizeof(MotorData_EEPROM),
				.timerFrequency = 1000,
				.stepSize = 203,
				.isReversed = false,
				.maxSpeed = 50,
				.positionZero = 0 * ACCURACY,
				.positionEnd = 20 * ACCURACY,
		},
};

MotorSettings motor4 = {
		.IOreset = { .PORT = MOT4_RESET_GPIO_Port, .PIN = MOT4_RESET_Pin },
		.IOsleep = { .PORT = MOT4_SLEEP_GPIO_Port, .PIN = MOT4_SLEEP_Pin },
		.IOdirection = { .PORT = MOT4_DIRECTION_GPIO_Port, .PIN = MOT4_DIRECTION_Pin },
		.IOstep = { .PORT = MOT4_STEP_GPIO_Port, .PIN = MOT4_STEP_Pin },

		.flags = {
				.isOn = 0,
				.reset = 0,
				.sleep = 0,
				.stepPhase = 0,
				.direction = 0,
		},

		.device = {
				.motorNum = 4,
				.eepromDataAddress = 0x00 + 2*sizeof(MotorData_EEPROM),
				.timerFrequency = 1000,
				.stepSize = 203,
				.isReversed = false,
				.maxSpeed = 50,
				.positionZero = 0 * ACCURACY,
				.positionEnd = 20 * ACCURACY,
		},
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

OuterComm_Settings outComm = {
		.Buff_InputCommands = NULL,
		.Buff_IN = NULL,
		.Buff_OUT = NULL,
		.huart = &huart2,
		.EOL_recieved = false,
		.transmission = false
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

DeviceSettings printerSettings;



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

void init_deviceSettings(DeviceSettings* settings)
{
	settings->positioningMode = ABSOLUTE;
	settings->sdCommandState = IDLE;

	settings->fatfs = &fatfs;
	settings->sd = &sd;
	settings->motors[0] = &motor1;
	settings->motors[1] = &motor2;
	settings->motors[2] = &motor3;
	settings->motors[3] = &motor4;
	settings->eeprom = &eeprom;
	settings->lcd = &lcd;
	settings->outComm = &outComm;

	settings->errMove = false;
	settings->speed = 0.0;
	settings->recievedBT = 0;
	settings->EOL_recieved = false;
	settings->transmissionBT = false;
}

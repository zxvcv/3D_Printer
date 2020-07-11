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



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

DeviceSettings printerSettings = {
		.positioningMode = ABSOLUTE,
		.sdCommandState = IDLE,
		.motors = motors,
		.eeprom = &eeprom,
		.lcd = &lcd,
		.errMove = false,
		.speed = 0.0
};




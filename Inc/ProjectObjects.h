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

#ifndef PROJECT_OBJECTS_H_
#define PROJECT_OBJECTS_H_



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include <stdbool.h>
#include "a4988_stepstick.h"
#include "EEPROM_24AA01.h"
#include "ST7565.h"
#include "diskio.h"
#include "BT.h"
#include "SD.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

//#define USE_INTERRUPTS
//#define LOG_ENABLE

#define MOTORS_NUM 4



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */

typedef struct MotorData_EEPROM{
	double maxSpeed;

	int stepSize;

	int positionZero;
	int positionEnd;
}MotorData_EEPROM;

typedef struct DeviceSettings_Tag{
	enum {
		RELATIVE,
		ABSOLUTE
	}positioningMode;

	enum {
		IDLE,
		BUSY
	}sdCommandState;

	FATFS* fatfs;

	SDCard_Settings* sd;

	MotorSettings* motors[MOTORS_NUM];

	EEPROMSettings* eeprom;

	ST7565R_Settings* lcd;

	BT_Settings* bt;

	bool errMove;

	double speed;
	uint8_t recievedBT;
	bool EOL_BT_recieved;
	bool transmissionBT;
}DeviceSettings;



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DECLARATIONS
 * ####################################################################################################### */



#endif /*PROJECT_OBJECTS_H_*/

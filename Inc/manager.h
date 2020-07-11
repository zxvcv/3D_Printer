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

#ifndef MANAGER_H_
#define MANAGER_H_



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include <stdbool.h>
#include "a4988_stepstick.h"
#include "EEPROM_24AA01.h"
#include "ST7565.h"
#include "diskio.h"
#include "managerBT.h"
#include "managerSDCard.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

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
		READY,
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

void init_manager(DeviceSettings* settings);

void clearAllMotorsRoundingErrors(DeviceSettings *settings);

void getMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns);

void setMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns, MotorData_EEPROM *data);



#endif /*MANAGER_H_*/

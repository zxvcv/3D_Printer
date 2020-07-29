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
#include "managerSDCard.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PRIVATE OBJECTS
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC OBJECTS
 * ####################################################################################################### */



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
	printerSettings->motors[0]->data.err.moveError = 0.0;
	printerSettings->motors[0]->data.err.speedError = 0.0;
	printerSettings->motors[1]->data.err.moveError = 0.0;
	printerSettings->motors[1]->data.err.speedError = 0.0;
	printerSettings->motors[2]->data.err.moveError = 0.0;
	printerSettings->motors[2]->data.err.speedError = 0.0;
	printerSettings->motors[3]->data.err.moveError = 0.0;
	printerSettings->motors[3]->data.err.speedError = 0.0;
}

void getMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns){
	MotorData_EEPROM data;
	EEPROM_readData(memSettigns, motSettings->device.eepromDataAddress, (uint8_t*)(&data), sizeof(MotorData_EEPROM));
	motSettings->device.stepSize = data.stepSize;
	motSettings->device.maxSpeed = data.maxSpeed;
	motSettings->device.positionZero = data.positionZero;
	motSettings->device.positionEnd = data.positionEnd;
}

void setMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns, MotorData_EEPROM *data){
	EEPROM_writeData(memSettigns, motSettings->device.eepromDataAddress, (uint8_t*)data, sizeof(MotorData_EEPROM));
}

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

Std_Err init_manager(DeviceSettings* settings)
{
	Std_Err stdErr;

	stdErr = init_operations_BT(settings->bt);
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	//Initialize SD Card
	HAL_GPIO_WritePin(SDSPI_CS_GPIO_Port, SDSPI_CS_Pin, GPIO_PIN_SET);

	stdErr = init_operations_SDcard(settings->sd);
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	return stdErr;
}

Std_Err clearAllMotorsRoundingErrors(DeviceSettings *settings)
{
	Std_Err stdErr = STD_OK;

	settings->motors[0]->data.err.moveError = 0.0;
	settings->motors[0]->data.err.speedError = 0.0;
	settings->motors[1]->data.err.moveError = 0.0;
	settings->motors[1]->data.err.speedError = 0.0;
	settings->motors[2]->data.err.moveError = 0.0;
	settings->motors[2]->data.err.speedError = 0.0;
	settings->motors[3]->data.err.moveError = 0.0;
	settings->motors[3]->data.err.speedError = 0.0;

	return stdErr;
}

Std_Err getMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns)
{
	Std_Err stdErr = STD_OK;

	MotorData_EEPROM data;
	stdErr = EEPROM_readData(memSettigns, motSettings->device.eepromDataAddress, (uint8_t*)(&data), sizeof(MotorData_EEPROM));
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	motSettings->device.stepSize = data.stepSize;
	motSettings->device.maxSpeed = data.maxSpeed;
	motSettings->device.positionZero = data.positionZero;
	motSettings->device.positionEnd = data.positionEnd;

	return stdErr;
}

Std_Err setMotorData_EEPROM(MotorSettings *motSettings, EEPROMSettings *memSettigns, MotorData_EEPROM *data)
{
	Std_Err stdErr = STD_OK;
	
	stdErr = EEPROM_writeData(memSettigns, motSettings->device.eepromDataAddress, (uint8_t*)data, 
		sizeof(MotorData_EEPROM));
	
	return stdErr;
}

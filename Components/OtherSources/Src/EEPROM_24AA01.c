/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 *  EEPROM_24AA01.c
 *
 *  Created on: 06.03.2020
 *      Author: ppisk
 * =======================================================================================================
 * COMMENTS:
 * 		Remember to use same Queue type to enqueue and dequeue.
 * 		If the functions do not have a compatible type there, memory leaks will occur.
 *
 * 		Don't mix type 1 and type 2 FIFO in one queue or memory leaks will occur.
 * =======================================================================================================
 * EXAMPLE:
 * 		None
 ********************************************************************************************************** */



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include "EEPROM_24AA01.h"
#include "main.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

#define _24AA01_ADDRESS 		((uint8_t)0xa0)

#define _24AA01_READ 			((uint8_t)0x01)
#define _24AA01_WRITE 			((uint8_t)0x00)

#define _24AA01_FIRST_ADDRESS 	((uint8_t)0x00)
#define _24AA01_LAST_ADDRESS 	((uint8_t)0x7F)
#define _24AA01_MEM_SIZE 		((uint16_t)128) //Bytes
#define _24AA01_PAGE_SIZE 		((uint16_t)8) 	//Bytes



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

Std_Err EEPROM_clear(EEPROMSettings *settings)
{
	Std_Err retVal = STD_ERROR;

	if(settings->isReady)
	{
		uint8_t clsTab[_24AA01_PAGE_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		uint8_t offset = 0;

		for(int i=0; i<_24AA01_MEM_SIZE/_24AA01_PAGE_SIZE; ++i)
		{
			while(HAL_I2C_Mem_Write(settings->i2c, 0xa0, offset, 1, clsTab, _24AA01_PAGE_SIZE, 
				HAL_MAX_DELAY) != HAL_OK);

			offset += _24AA01_PAGE_SIZE;
		}
		retVal = STD_OK;
	}
	else
	{
		retVal = STD_BUSY_ERROR;
	}
	
	return retVal;
}

Std_Err EEPROM_writeData(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size)
{
	Std_Err retVal = STD_ERROR;

	if(!settings->isReady)
	{
		retVal = STD_BUSY_ERROR;
	}
	else if(size <= 0)
	{
		retVal = STD_PARAMETER_ERROR;
	}
	else
	{
		if(address + size > _24AA01_MEM_SIZE) size = _24AA01_MEM_SIZE - address;

		uint8_t offset = 0;
		uint8_t sendSize = _24AA01_PAGE_SIZE - (address % _24AA01_PAGE_SIZE);
		if(sendSize == 0) sendSize = _24AA01_PAGE_SIZE;
		if(sendSize > size) sendSize = size;

		while(HAL_I2C_Mem_Write(settings->i2c, _24AA01_ADDRESS, address, 1, data, sendSize, HAL_MAX_DELAY) != HAL_OK);
		offset += sendSize;
		size -= sendSize;

		while(size > 0){
			while(HAL_I2C_Mem_Write(settings->i2c, _24AA01_ADDRESS, address + offset, 1, data + offset, size > _24AA01_PAGE_SIZE ? _24AA01_PAGE_SIZE : size, HAL_MAX_DELAY) != HAL_OK);
			offset += _24AA01_PAGE_SIZE; size -= _24AA01_PAGE_SIZE;
		}

		retVal = STD_OK;
	}

	return retVal;
}

Std_Err EEPROM_readData(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size)
{
	Std_Err retVal = STD_OK;

	if(!settings->isReady)
	{
		retVal = STD_BUSY_ERROR;
	}
	else if(size <= 0)
	{
		retVal = STD_PARAMETER_ERROR;
	}
	else
	{
		if(address + size > _24AA01_MEM_SIZE) size = _24AA01_MEM_SIZE - address;
		HAL_StatusTypeDef halErr;
		halErr = HAL_I2C_Mem_Read(settings->i2c, _24AA01_ADDRESS, address, 1, data, size, HAL_MAX_DELAY);
		retVal = translate_error_hal_to_project(halErr);
	}

	return retVal;
}

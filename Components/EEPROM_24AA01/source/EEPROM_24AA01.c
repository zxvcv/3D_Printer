/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: EEPROM_24AA01
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "EEPROM_24AA01.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define _24AA01_ADDRESS         ((uint8_t)0xa0)

#define _24AA01_READ            ((uint8_t)0x01)
#define _24AA01_WRITE           ((uint8_t)0x00)

#define _24AA01_FIRST_ADDRESS   ((uint8_t)0x00)
#define _24AA01_LAST_ADDRESS    ((uint8_t)0x7F)
#define _24AA01_MEM_SIZE        ((uint16_t)128) //Bytes
#define _24AA01_PAGE_SIZE       ((uint16_t)8)   //Bytes
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void EEPROM_init(EEPROMSettings *settings, I2C_HandleTypeDef* i2c)
{
    settings->isReady = true;
    settings->i2c = i2c;
}


Std_Err EEPROM_clear(EEPROMSettings *settings)
{
    Std_Err retVal = STD_ERROR;
    HAL_StatusTypeDef halErr;

    if(settings->isReady)
    {
        uint8_t clsTab[_24AA01_PAGE_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        uint8_t offset = 0;

        for(int i=0; i<_24AA01_MEM_SIZE/_24AA01_PAGE_SIZE; ++i)
        {
            do
            {
                halErr = HAL_I2C_Mem_Write(settings->i2c, 0xa0, offset, 1, clsTab,
                    _24AA01_PAGE_SIZE, HAL_MAX_DELAY);
            }
            while(halErr != HAL_OK);
            retVal = translate_error_hal_to_project(halErr);
            if(retVal != STD_OK) { return retVal; }

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
    HAL_StatusTypeDef halErr;

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
        if(address + size > _24AA01_MEM_SIZE)
        {
            size = _24AA01_MEM_SIZE - address;
        }

        uint8_t offset = 0;
        uint8_t sendSize = _24AA01_PAGE_SIZE - (address % _24AA01_PAGE_SIZE);

        if(sendSize == 0)
        {
            sendSize = _24AA01_PAGE_SIZE;
        }
        if(sendSize > size)
        {
            sendSize = size;
        }

        do
        {
            halErr = HAL_I2C_Mem_Write(settings->i2c, _24AA01_ADDRESS, address, 1, data, sendSize,
                HAL_MAX_DELAY);
        }
        while(halErr != HAL_OK);
        retVal = translate_error_hal_to_project(halErr);
        if(retVal != STD_OK) { return retVal; }

        offset += sendSize;
        size -= sendSize;

        while(size > 0){
            do
            {
                halErr = HAL_I2C_Mem_Write(settings->i2c, _24AA01_ADDRESS, address + offset, 1,
                    data + offset, size > _24AA01_PAGE_SIZE ? _24AA01_PAGE_SIZE : size,
                    HAL_MAX_DELAY);
            }
            while(halErr != HAL_OK);

            retVal = translate_error_hal_to_project(halErr);
            if(retVal != STD_OK) { return retVal; }

            offset += _24AA01_PAGE_SIZE;
            size -= _24AA01_PAGE_SIZE;
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
        do
        {
            halErr = HAL_I2C_Mem_Read(settings->i2c, _24AA01_ADDRESS, address, 1, data, size,
                HAL_MAX_DELAY);
        }
        while(halErr != HAL_OK);
        retVal = translate_error_hal_to_project(halErr);
    }

    return retVal;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

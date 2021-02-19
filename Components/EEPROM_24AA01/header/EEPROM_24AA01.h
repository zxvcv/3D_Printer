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
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      None
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef EEPROM_24AA01_H_
#define EEPROM_24AA01_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include <stdbool.h>
#include <stdint.h>
#include "Error_Codes.h"
#include "main.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct EEPROMSettings{
    bool isReady;
    I2C_HandleTypeDef* i2c;
} EEPROMSettings;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

Std_Err EEPROM_clear(EEPROMSettings *settings);

Std_Err EEPROM_writeData(EEPROMSettings *settings, uint8_t address, uint8_t *data, int size);

Std_Err EEPROM_readData(EEPROMSettings *settings, uint8_t address, uint8_t *dataOUT, int size);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* EEPROM_24AA01_H_ */

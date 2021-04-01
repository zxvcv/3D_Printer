/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Manager_EEPROM
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Manager_EEPROM.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err get_motor_data_EEPROM(EEPROMSettings* eeprom_settigns, uint8_t address,
    MotorData_EEPROM* dataOUT)
{
    return EEPROM_readData(eeprom_settigns, address, (uint8_t*)dataOUT, sizeof(MotorData_EEPROM));
}


Std_Err set_motor_data_EEPROM(EEPROMSettings *eeprom_settigns, uint8_t address,
    MotorData_EEPROM *data)
{
    return EEPROM_writeData(eeprom_settigns, address, (uint8_t*)data, sizeof(MotorData_EEPROM));
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

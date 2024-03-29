/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Project_Objects
 *      [[COMPONENT_DESCRIPTION]]
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef PROJECT_OBJECTS_H_
#define PROJECT_OBJECTS_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "SD.h"
#include "A4988_stepstick.h"
#include "EEPROM_24AA01.h"
#include "Manager_Communication.h"
#include "Manager_SDcard.h"
#include "Manager_BoundariesDetector.h"
#include "Project_Config.h"
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

typedef struct DeviceSettings_Tag{
    SDCard_Settings* sd;

    Motor* motors[MOTORS_NUM];
    uint8_t motor_data_addresses[MOTORS_NUM];
    bool motors_are_on;

    EEPROMSettings* eeprom;

    BuffCommunication_Settings* buff_comm;
    Communication_Settings* communication;

    BoundariesDetector_Settings* boundaryDetection;

    char msg_buffer[50];
    Std_Err error;
}DeviceSettings;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void init_deviceSettings(DeviceSettings* settings);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* PROJECT_OBJECTS_H_ */

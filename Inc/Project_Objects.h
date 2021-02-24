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
#include "Buffered_Communication.h"
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

    OuterComm_Settings* outComm;

    bool errMove;

    double speed;
    uint8_t recievedBT;
    bool EOL_recieved; /*TODO: check if it is not the same as EOL_resieved in outComm struct*/
    bool transmissionBT;
}DeviceSettings;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void init_deviceSettings(DeviceSettings* settings);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* PROJECT_OBJECTS_H_ */

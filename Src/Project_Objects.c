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
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Project_Objects.h"
#include "Manager_EEPROM.h"
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
 *                                      EXTERNS                                                 *
 * ############################################################################################ */


extern SPI_HandleTypeDef hspi2;




/* ############################################################################################ *
 *                                      PRIVATE OBJECTS                                         *
 * ############################################################################################ */


FATFS fatfs;

//[TODO]: create init function for SDCard_Settings
SDCard_Settings sd = {
    .file = &file,
    .BuffIN_SDcmd = NULL,
    .end_SDprogram = false,
    .executing_SDprogram = false,
    .executing_SDcommand = false,

    #ifdef LOG_ENABLE
    .logFile = &logFile,
    .BuffOUT_logs = NULL
    #endif /* LOG_ENABLE */
};

Motor motor1;
Motor motor2;
Motor motor3;
Motor motor4;

EEPROMSettings eeprom;

BuffCommunication_Settings buff_comm;

// FIL file;

// #ifdef LOG_ENABLE
// FIL logFile;
// #endif /* LOG_ENABLE */





/* ############################################################################################ *
 *                                      PUBLIC OBJECTS                                          *
 * ############################################################################################ */

DeviceSettings printerSettings;



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void init_deviceSettings(DeviceSettings* settings)
{
    settings->fatfs = &fatfs;

    settings->sd = &sd;

    settings->motors[0] = &motor1;
    settings->motors[1] = &motor2;
    settings->motors[2] = &motor3;
    settings->motors[3] = &motor4;

    /* eeprom addresses setup */
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        settings->motor_data_addresses[i] = 0x00 + i * sizeof(MotorData_EEPROM);
    }

    settings->eeprom = &eeprom;

    settings->buff_comm = &buff_comm;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

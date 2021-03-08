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

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart2;



/* ############################################################################################ *
 *                                      PRIVATE OBJECTS                                         *
 * ############################################################################################ */

MotorSettings motor1 = {
    .device = {
            .eepromDataAddress = 0x00,
    },
};

MotorSettings motor2 = {
    .device = {
            .eepromDataAddress = 0x00 + sizeof(MotorData_EEPROM),
    },
};

MotorSettings motor3 = {
    .device = {
        .eepromDataAddress = 0x00 + 2*sizeof(MotorData_EEPROM),
    },
};

MotorSettings motor4 = {
    .device = {
            .eepromDataAddress = 0x00 + 2*sizeof(MotorData_EEPROM),
    },
};

EEPROMSettings eeprom = {
    .isReady = true,
    .i2c = &hi2c1
};

OuterComm_Settings outComm = {
    .Buff_InputCommands = NULL,
    .Buff_IN = NULL,
    .Buff_OUT = NULL,
    .huart = &huart2,
    .EOL_recieved = false,
    .transmission = false
};

FATFS fatfs;

FIL file;

#ifdef LOG_ENABLE
FIL logFile;
#endif /* LOG_ENABLE */

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



/* ############################################################################################ *
 *                                      PUBLIC OBJECTS                                          *
 * ############################################################################################ */

DeviceSettings printerSettings;



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void init_deviceSettings(DeviceSettings* settings)
{
    settings->positioningMode = ABSOLUTE;
    settings->sdCommandState = IDLE;

    settings->fatfs = &fatfs;
    settings->sd = &sd;
    settings->motors[0] = &motor1;
    settings->motors[1] = &motor2;
    settings->motors[2] = &motor3;
    settings->motors[3] = &motor4;
    settings->eeprom = &eeprom;
    settings->outComm = &outComm;

    settings->errMove = false;
    settings->speed = 0.0;
    settings->recievedBT = 0;
    settings->EOL_recieved = false;
    settings->transmissionBT = false;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

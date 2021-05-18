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
 *                                      EXTERNS                                                 *
 * ############################################################################################ */


extern SPI_HandleTypeDef hspi2;




/* ############################################################################################ *
 *                                      PRIVATE OBJECTS                                         *
 * ############################################################################################ */

SDCard_Settings sd;

Motor motor1;
Motor motor2;
Motor motor3;
Motor motor4;

EEPROMSettings eeprom;

BuffCommunication_Settings buff_comm;
Communication_Settings communication;

BoundariesDetector_Settings boundaryDetection;



/* ############################################################################################ *
 *                                      PUBLIC OBJECTS                                          *
 * ############################################################################################ */

DeviceSettings printerSettings;



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void init_deviceSettings(DeviceSettings* settings)
{
    settings->sd = &sd;

    settings->motors[MOTOR_X] = &motor1;
    settings->motors[MOTOR_Y] = &motor2;
    settings->motors[MOTOR_Z] = &motor3;
    settings->motors[MOTOR_E] = &motor4;

    /* eeprom addresses setup */
    for(int i=0; i<MOTORS_NUM; ++i)
    {
        settings->motor_data_addresses[i] = 0x00 + i * sizeof(MotorData_EEPROM);
    }

    settings->eeprom = &eeprom;

    settings->buff_comm = &buff_comm;
    settings->communication = &communication;

    settings->boundaryDetection = &boundaryDetection;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

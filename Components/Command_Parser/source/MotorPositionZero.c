/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Command_Parser
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "_commands.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

Std_Err systemCmd_MotorPositionZero(SystemCommand* cmd, DeviceSettings* settings)
{
    Std_Err stdErr;
    /*TODO: distinguishing between errors*/

    int argInt = (int)(cmd->arg[0] * ACCURACY);
    cmd->motor[0]->device.positionZero = argInt;
    stdErr = EEPROM_writeData(settings->eeprom, 
            cmd->motor[0]->device.eepromDataAddress + _OFFSET_POSITIONZERO, 
            (uint8_t*)(&argInt), 
            sizeof(argInt));
    /*TODO: read data form EEPROM again*/
    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    stdErr = systemCmd_MotorDataRequest(cmd, settings);
    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

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

Std_Err systemCmd_MotorSpeedMax(SystemCommand* cmd, DeviceSettings* settings)
{
    Std_Err stdErr;
    /*TODO: distinguishing between errors*/

    for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
    {
        if(cmd->arg[0] >= 0)
        {
            cmd->motor[i]->device.maxSpeed = cmd->arg[0];
        }
            
        stdErr = EEPROM_writeData(settings->eeprom, 
            cmd->motor[i]->device.eepromDataAddress + _OFFSET_MAXSPEED, 
            (uint8_t*)(cmd->arg), 
            sizeof(double));

        if(stdErr != STD_OK)
        {
            return stdErr;
        }
    }

    stdErr = systemCmd_MotorDataRequest(cmd, settings);
    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

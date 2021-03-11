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

Std_Err init_U25(SystemCommand* cmd)
{
    cmd->delete = NULL;
    cmd->step = NULL;

    // Std_Err stdErr;
    // /*TODO: distinguishing between errors*/

    // int argInt = (int)(cmd->arg[0] * ACCURACY);
    // cmd->motor[0]->device.stepSize = argInt;
    // stdErr = EEPROM_writeData(settings->eeprom,
    //         cmd->motor[0]->device.eepromDataAddress + _OFFSET_STEPSIZE,
    //         (uint8_t*)(&argInt),
    //         sizeof(argInt));
    // if(stdErr != STD_OK)
    // {
    //     return stdErr;
    // }

    // stdErr = systemCmd_MotorStepSizeRequest(cmd, settings);
    // return stdErr;

    return STD_OK;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

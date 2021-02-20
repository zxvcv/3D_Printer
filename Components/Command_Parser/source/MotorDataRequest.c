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

Std_Err systemCmd_MotorDataRequest(SystemCommand* cmd, DeviceSettings* settings)
{
    Std_Err stdErr = STD_ERROR;

    for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
    {
        msgSize = sprintf(buffMsg, "DT M%d %f %f %f %f %f\n", 
                cmd->motor[i]->device.motorNum, 
                (double)cmd->motor[i]->data.position / ACCURACY, 
                (double)cmd->motor[i]->device.positionZero / ACCURACY,
                (double)cmd->motor[i]->device.positionEnd / ACCURACY, 
                cmd->motor[i]->data.speed, 
                cmd->motor[i]->device.maxSpeed);
        
        stdErr = fifo_push_C(settings->outComm->Buff_OUT, (char*)buffMsg, msgSize);
        if(stdErr != STD_OK)
        {
            return stdErr;
        }
    }

    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

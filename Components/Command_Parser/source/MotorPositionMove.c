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

Std_Err systemCmd_MotorPositionMove(SystemCommand* cmd, DeviceSettings* settings)
{
    Std_Err stdErr;

    double move = cmd->arg[0] - ((double)cmd->motor[0]->data.position / ACCURACY);

    for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
    {
        stdErr = motorSetMove(cmd->motor[i], move, &(settings->motors[i]->data.err));

        if(stdErr != STD_OK)
        {
            return stdErr;
        }
    }

    #ifdef USE_INTERRUPTS
    IRQ_DISABLE;
    #endif /* USE_INTERRUPTS */

    for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
    {
        stdErr = motorStart(cmd->motor[i]);

        if(stdErr != STD_OK)
        {
            for(int j=0; j < cmd->motorsNum && j < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++j)\
            {
                motorStop(cmd->motor[j]);
            }
            return stdErr;
        }
    }

    #ifdef USE_INTERRUPTS
    IRQ_ENABLE;
    #endif /* USE_INTERRUPTS */

    bool state;
    do
    {
        state = false;
        for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
        {
            state |= motorIsOn(cmd->motor[i]);
        }
    }while(state);

    stdErr = systemCmd_MotorDataRequest(cmd, settings);
    return stdErr;
}
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

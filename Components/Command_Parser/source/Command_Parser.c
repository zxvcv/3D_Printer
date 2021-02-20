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
#include <string.h>
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

const struct {
    char* name;
    Std_Err (*execute)(SystemCommand*, DeviceSettings*);
} systemCommands[SYSTEM_COMMANDS_NUM] = {
        {   "DR",   systemCmd_MotorDataRequest      },
        {   "PM",   systemCmd_MotorPositionMove     },
        {   "PV",   systemCmd_MotorPositionValueSet },
        {   "PZ",   systemCmd_MotorPositionZero     },
        {   "PE",   systemCmd_MotorPositionEnd      },
        {   "DM",   systemCmd_MotorDistanceMove     },
        {   "SS",   systemCmd_MotorSpeedSet         },
        {   "SM",   systemCmd_MotorSpeedMax         },
        {   "SR",   systemCmd_MotorStepSizeRequest  },
        {   "SP",   systemCmd_MotorStepSizeSet      },
        {   "CR",   systemCmd_SDCardProgramRun      }
};
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err parseSystemCommand(char* cmd, SystemCommand* cmdOUT, DeviceSettings* settings)
{
    Std_Err stdErr = STD_ERROR;
    char *token = NULL, *cmdName = NULL, *motorNum = NULL, *num = NULL;
    int  argNum = 0;

    memset(cmdOUT, 0, sizeof(SystemCommand));

    //command name
    cmdName = strtok(cmd, " ");

    for (int i = 0; i < SYSTEM_COMMANDS_NUM; ++i)
    {
        if (strcmp(cmdName, systemCommands[i].name) == 0)
        {
            cmdOUT->execute = systemCommands[i].execute;
            stdErr = STD_OK;
            break;
        }
    }

    if(stdErr == STD_ERROR)
    {
        return stdErr;
    }

    //command subtype
    motorNum = strtok(NULL, " ");

    if(motorNum != NULL && motorNum[0] == 'M')
    {
        //motor(s) number
        uint8_t val = 0;
        for(int i=0; motorNum[i + 1] != '\0' && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i, ++val)
        {
            switch(motorNum[i + 1])
            {
            case '1': cmdOUT->motor[i] = settings->motors[0]; break;
            case '2': cmdOUT->motor[i] = settings->motors[1]; break;
            case '3': cmdOUT->motor[i] = settings->motors[2]; break;
            case '4': cmdOUT->motor[i] = settings->motors[3]; break;
            default: cmdOUT->motor[i] = NULL; break;
            }
        }

        cmdOUT->motorsNum = val;

        //motor command arguments
        token = strtok(NULL, " ");
        while (token != NULL && argNum < SYSTEM_COMMANDS_ARGS_MAX_NUM)
        {
            num = token;
            cmdOUT->arg[argNum++] = strtod(num, NULL);

            token = strtok(NULL, " ");
        }
    }

    return stdErr;
}


Std_Err executeSystemCommand(SystemCommand* cmd, DeviceSettings* settings)
{
    Std_Err stdErr;

    if (cmd->execute == NULL)
    {
        return STD_PARAMETER_ERROR;
    }

    stdErr = cmd->execute(cmd, settings);
    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

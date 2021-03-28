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
#include <stdlib.h>
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define SYSTEM_COMMANDS_NUM 12
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

const struct {
    char* name;
    Std_Err (*execute)(SystemCommand*);
} _system_commands[SYSTEM_COMMANDS_NUM] = {
        {   "U00",      init_U00    },// "DR",   systemCmd_MotorDataRequest
        {   "U01",      init_U01    },// ----,   systemCmd_MotorSettingsRequest
        {   "U02",      init_U02    },// ----,   systemCmd_MotorFlagsRequest
        {   "U10",      init_U10    },// "PM",   systemCmd_MotorLinearAbsoluteMove
        {   "U11",      init_U11    },// "DM",   systemCmd_MotorLinearRelativeMove
        {   "U20",      init_U20    },// "PV",   systemCmd_MotorPositionValueSet
        {   "U21",      init_U21    },// "PZ",   systemCmd_MotorPositionZero
        {   "U22",      init_U22    },// "PE",   systemCmd_MotorPositionEnd
        {   "U23",      init_U23    },// "SM",   systemCmd_MotorSpeedMax
        {   "U24",      init_U24    },// "SP",   systemCmd_MotorStepSizeSet
        {   "U40",      init_U40    } // "CR",   systemCmd_SDCardProgramRun
};
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void init_SystemCommandsParser(BuffCommunication_Settings* buff_comm, Motor* motors)
{
    global_systemCmd_settings.buff_comm = buff_comm;
    global_systemCmd_settings.motors = motors;
}


Std_Err parse_SystemCommand(char* cmd, SystemCommand* cmdOUT)
{
    Std_Err stdErr = STD_ERROR;
    char* token = NULL, * cmdName = NULL;
    double val;

    cmdName = strtok(cmd, " ");
    token = strtok(NULL, " ");
    memset(cmdOUT, 0, sizeof(SystemCommand));

    switch (cmdName[0])
    {
        case '0': cmdOUT->execution_policy = PRIORITY; break;
        case '1': cmdOUT->execution_policy = NORMAL;   break;
        default: return STD_PARAMETER_ERROR;
    }

    for (int i = 0; i < SYSTEM_COMMANDS_NUM; ++i)
    {
        if (strcmp(cmdName + 1, _system_commands[i].name) == 0)
        {
            cmdOUT->init = _system_commands[i].execute;
            stdErr = STD_OK;
            break;
        }
    }

    if(stdErr != STD_OK)
    {
        return stdErr;
    }

    while (token != NULL)
    {
        val = atoi(token + 1);
        switch (token[0])
        {
            case 'X': cmdOUT->data.x = val; cmdOUT->used_fields |= PARAM_X; break;
            case 'Y': cmdOUT->data.y = val; cmdOUT->used_fields |= PARAM_Y; break;
            case 'Z': cmdOUT->data.z = val; cmdOUT->used_fields |= PARAM_Z; break;
            case 'E': cmdOUT->data.e = val; cmdOUT->used_fields |= PARAM_E; break;
            case 'F': cmdOUT->data.f = val; cmdOUT->used_fields |= PARAM_F; break;
            default: break;
        }
        token = strtok(NULL, " ");
    }

    return stdErr;
}
/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

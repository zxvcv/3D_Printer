/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: GCode_Parser
 *      [[COMPONENT_DESCRIPTION]]
 ************************************************************************************************/


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "GCode_Parser.h"
#include "_commands.h"
#include <string.h>
#include "A4988_stepstick.h"
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define GCODE_COMMANDS_NUM 10
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

GCodeGlobal global_gcode_settings;

const struct {
    char* name;
    Std_Err (*execute)(GCodeCommand*, DeviceSettings*);
} commands[GCODE_COMMANDS_NUM] = {
        {   "G1",   command_G1      },
        {   "G28",  command_G28     },
        {   "G90",  command_G90     },
        {   "G91",  command_G91     },
        {   "G92",  command_G92     },
        {   "M104", command_M104    },
        {   "M109", command_M109    },
        {   "M140", command_M140    },
        {   "M190", command_M190    },
        {   "M106", command_M106    }
};
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

Std_Err parseGCodeCommand(char* cmd, GCodeCommand* cmdOUT)
{
    Std_Err stdErr = STD_ERROR;
    char* token = NULL, * cmdName = NULL;
    double val;

    cmdName = strtok(cmd, " ");
    token = strtok(NULL, " ");
    memset(cmdOUT, 0, sizeof(GCodeCommand));

    for (int i = 0; i < GCODE_COMMANDS_NUM; ++i) {
        if (strcmp(cmdName, commands[i].name) == 0) {
            cmdOUT->init = commands[i].execute;
            stdErr = STD_OK;
            break;
        }
    }

    if(stdErr == STD_ERROR)
    {
        return stdErr;
    }

    while (token != NULL) {
        val = atoi(token + 1);
        switch (token[0])
        {
        case 'X': cmdOUT->data.x = val; cmdOUT->usedFields._x = 1; break;
        case 'Y': cmdOUT->data.y = val; cmdOUT->usedFields._y = 1; break;
        case 'Z': cmdOUT->data.z = val; cmdOUT->usedFields._z = 1; break;
        case 'E': cmdOUT->data.e = val; cmdOUT->usedFields._e = 1; break;
        case 'F': cmdOUT->data.f = val; cmdOUT->usedFields._f = 1; break;
        case 'S': cmdOUT->data.s = val; cmdOUT->usedFields._s = 1; break;
        default: break;
        }
        token = strtok(NULL, " ");
    }

    return stdErr;
}

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

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
#include <stdlib.h>
/*[[COMPONENT_INCLUDES_C]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define GCODE_COMMANDS_NUM 12
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

GCodeGlobal global_gcode_settings;

const struct {
    char* name;
    Std_Err (*execute)(GCodeCommand*);
} _gcode_commands[GCODE_COMMANDS_NUM] = {
        {   "G1",   init_G1         },
        {   "G2",   init_G2         },
        {   "G3",   init_G3         },
        {   "G28",  init_G28        },
        {   "G90",  init_G90        },
        {   "G91",  init_G91        },
        {   "G92",  init_G92        },
        {   "M104", init_M104       },
        {   "M106", init_M106       },
        {   "M109", init_M109       },
        {   "M140", init_M140       },
        {   "M190", init_M190       }
};
/*[[COMPONENT_PRIVATE_DEFINITIONS]]*/



/* ############################################################################################ *
 *                                      PUBLIC DEFINITIONS                                      *
 * ############################################################################################ */

void init_GCodeParser(Motor** motors, BuffCommunication_Settings* buff_comm, bool* motors_are_on)
{
    global_gcode_settings.motors = motors;
    global_gcode_settings.buff_comm = buff_comm; // DEBUG
    global_gcode_settings.motors_are_on = motors_are_on;
    global_gcode_settings.positioning_mode = ABSOLUTE;
    global_gcode_settings.speed = 0.0;
}


Std_Err parse_GCodeCommand(char* cmd, GCodeCommand* cmdOUT)
{
    Std_Err stdErr = STD_ERROR;
    char* token = NULL, * cmdName = NULL;
    double val;

    cmdName = strtok(cmd, " ");
    token = strtok(NULL, " ");
    memset(cmdOUT, 0, sizeof(GCodeCommand));

    for (int i = 0; i < GCODE_COMMANDS_NUM; ++i) {
        if (strcmp(cmdName, _gcode_commands[i].name) == 0) {
            cmdOUT->init = _gcode_commands[i].execute;
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
            case 'X': cmdOUT->data.x = val; cmdOUT->used_fields |= PARAM_X; break;
            case 'Y': cmdOUT->data.y = val; cmdOUT->used_fields |= PARAM_Y; break;
            case 'Z': cmdOUT->data.z = val; cmdOUT->used_fields |= PARAM_Z; break;
            case 'E': cmdOUT->data.e = val; cmdOUT->used_fields |= PARAM_E; break;
            case 'F': cmdOUT->data.f = val; cmdOUT->used_fields |= PARAM_F; break;
            case 'S': cmdOUT->data.s = val; cmdOUT->used_fields |= PARAM_S; break;
            default: break;
        }
        token = strtok(NULL, " ");
    }

    return stdErr;
}

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

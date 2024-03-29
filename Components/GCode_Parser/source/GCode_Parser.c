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

#define GCODE_COMMANDS_NUM 15
/*[[COMPONENT_DEFINES_C]]*/



/* ############################################################################################ *
 *                                      PRIVATE DEFINITIONS                                     *
 * ############################################################################################ */

const struct {
    char* name;
    Std_Err (*execute)(GCode_Settings*, GCodeCommand*);
} _gcode_commands[GCODE_COMMANDS_NUM] = {
        {   "G1",   init_G1         },
        {   "G2",   init_G2         },
        {   "G3",   init_G3         },
        {   "G17",  init_G17        },
        {   "G18",  init_G18        },
        {   "G19",  init_G19        },
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

void init_GCodeParser(GCode_Settings* settings, Motor** motors,
    BuffCommunication_Settings* buff_comm, bool* motors_are_on)
{
    settings->motors = motors;
    settings->buff_comm = buff_comm;
    settings->motors_are_on = motors_are_on;
    settings->positioning_mode = RELATIVE;
    settings->speed = 1.;
    settings->angle_step = 1.;
    settings->circle_move_mode = CLOCKWISE_CIRCLE;
    settings->plane_selection.plane_x = 1;
    settings->plane_selection.plane_y = 1;
    settings->plane_selection.plane_z = 0;
}


Std_Err parse_GCodeCommand(GCode_Settings* settings, char* cmd, GCodeCommand* cmdOUT)
{
    Std_Err stdErr = STD_COMMAND_ERROR;
    char* token = NULL, * cmdName = NULL;
    double val;

    // delete all comments
    bool isComment = false;
    for (int i = 0; i < strlen(cmd); ++i)
    {
        if(cmd[i] == ';')
        {
            isComment = true;
        }

        if(isComment && cmd[i] != '\n')
        {
            cmd[i] = ' ';
        }
    }

    // if empty line then pass
    cmdName = strtok(cmd, " ");

    // parse command
    token = strtok(NULL, " ");
    memset(cmdOUT, 0, sizeof(GCodeCommand));

    for (int i = 0; i < GCODE_COMMANDS_NUM; ++i) {
        if (strcmp(cmdName, _gcode_commands[i].name) == 0) {
            cmdOUT->init = _gcode_commands[i].execute;
            stdErr = STD_OK;
            break;
        }
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
            case 'I': cmdOUT->data.i = val; cmdOUT->used_fields |= PARAM_I; break;
            case 'J': cmdOUT->data.j = val; cmdOUT->used_fields |= PARAM_J; break;
            case 'K': cmdOUT->data.k = val; cmdOUT->used_fields |= PARAM_K; break;
            default: break;
        }
        token = strtok(NULL, " ");
    }

    return stdErr;
}

/*[[COMPONENT_PUBLIC_DEFINITIONS]]*/

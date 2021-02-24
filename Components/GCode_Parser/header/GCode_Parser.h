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
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef GCODE_PARSER_H_
#define GCODE_PARSER_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include <stdbool.h>
#include "Error_Codes.h"
#include "Vector_Math.h"
#include "Project_Objects.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define MOTOR_X     0 //[DEBUG]
#define MOTOR_Y     1 //[DEBUG]
#define MOTOR_Z1    2 //[DEBUG]
#define MOTOR_Z2    3 //[DEBUG]
#define MOTOR_E     4 //[DEBUG]
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct GCodeCommand_Tag{
    Std_Err (*execute)(struct GCodeCommand_Tag*, DeviceSettings*) ;	//command pointer
    int cmdNum;

    struct{
        double x;       //X-axis move
        double y;       //Y-axis move
        double z;       //Z-axis move
        double e;       //extruder-axis move
        double f;       //speed of the movement
        double s;       //temperature
    }data;

    struct{
        unsigned short _x : 1;
        unsigned short _y : 1;
        unsigned short _z : 1;
        unsigned short _e : 1;
        unsigned short _f : 1;
        unsigned short _s : 1;
    }usedFields;
}GCodeCommand;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

Std_Err parseGCodeCommand(char* cmd, GCodeCommand* cmdOUT);

Std_Err executeGCodeCommand(GCodeCommand* cmd, DeviceSettings* settings);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* GCODE_PARSER_H_ */

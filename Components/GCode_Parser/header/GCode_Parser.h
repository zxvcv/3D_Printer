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
#define MOTOR_Z     2 //[DEBUG]
#define MOTOR_E     3 //[DEBUG]
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct GCodeGlobal_Tag{
    Motor* motors;
    enum{
        RELATIVE,
        ABSOLUTE
    }positioning_mode;
    double speed;
}GCodeGlobal;

typedef struct GCodeCommand_Tag{
    Std_Err (*init)(struct GCodeCommand_Tag*);
    Std_Err (*delete)(struct GCodeCommand_Tag*);

    Std_Err (*step)(struct GCodeCommand_Tag*);


    uint8_t used_fields;
    struct{
        double x;       //X-axis move
        double y;       //Y-axis move
        double z;       //Z-axis move
        double e;       //extruder-axis move
        double f;       //speed of the movement
        double s;       //temperature
    }data;

    struct{
        double x;
        double y;
        double z;
        double e;
    }target_position;
}GCodeCommand;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void init_GCodeParser(Motor* motors);

Std_Err parse_GCodeCommand(char* cmd, GCodeCommand* cmdOUT);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* GCODE_PARSER_H_ */

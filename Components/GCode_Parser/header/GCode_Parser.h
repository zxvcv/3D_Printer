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
#include "A4988_stepstick.h"
#include "Buffered_Communication.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct GCode_Settings_Tag{
    Motor** motors;
    BuffCommunication_Settings* buff_comm;

    bool* motors_are_on;
    double speed;
    double angle_step;

    enum{
        RELATIVE = 0,
        ABSOLUTE
    }positioning_mode;

    enum{
        CLOCKWISE_CIRCLE = 0,
        COUNTER_CLOCKWISE_CIRCLE
    }circle_move_mode;

    struct{
        unsigned int plane_x    :1;
        unsigned int plane_y    :1;
        unsigned int plane_z    :1;
    }plane_selection;
}GCode_Settings;

typedef struct GCodeCommand_Tag{
    Std_Err (*init)(GCode_Settings*, struct GCodeCommand_Tag*);
    Std_Err (*remove)(GCode_Settings*, struct GCodeCommand_Tag*);
    Std_Err (*step)(GCode_Settings*, struct GCodeCommand_Tag*);


    uint16_t used_fields;
    struct{
        double x;       //X-axis move
        double y;       //Y-axis move
        double z;       //Z-axis move
        double e;       //extruder-axis move
        double f;       //speed of the movement
        double s;       //temperature
        double i;       //X-axis relative circle center position form start point
        double j;       //Y-axis relative circle center position form start point
        double k;       //Z-axis relative circle center position form start point
    }data;
    void* specific_data;

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

void init_GCodeParser(GCode_Settings* settings, Motor** motors,
    BuffCommunication_Settings* buff_comm, bool* motors_are_on);

Std_Err parse_GCodeCommand(GCode_Settings* settings, char* cmd, GCodeCommand* cmdOUT);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* GCODE_PARSER_H_ */

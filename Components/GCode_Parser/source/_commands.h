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

#ifndef _COMMANDS_H_
#define _COMMANDS_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "GCode_Parser.h"
#include <math.h>
#include <stdio.h>
#include "Vector_Math.h"
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define MSG_BUFF_SIZE 100

/* used_fields */
#define PARAM_X     0x01
#define PARAM_Y     0x02
#define PARAM_Z     0x04
#define PARAM_E     0x08
#define PARAM_F     0x10
#define PARAM_S     0x20
#define PARAM_LAST  PARAM_S
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct GCodeGlobal_Tag{
    BuffCommunication_Settings* buff_comm; // DEBUG
    Motor** motors;
    enum{
        RELATIVE,
        ABSOLUTE
    }positioning_mode;
    double speed;
}GCodeGlobal;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

extern GCodeGlobal global_gcode_settings;
/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      PRIVATE DECLARATIONS                                    *
 * ############################################################################################ */

/*
 * COMMAND G1
 * Linear Movement
 *
 *  Arguments:
 *   Use X, Y, or Z values to tell the printer what position to move to.
 *   These values will obey the current positioning mode (absolute or relative coordinates).
 *   Include an E value if you want to move the extruder as well.
 *   Finally, you can use an F value to tell the printer what speed (mm/min) to use for the movement.
 */
Std_Err init_G1(GCodeCommand* cmd);

/*
 * COMMAND G28
 * Return to home position (machine zero, aka machine reference point)
 *
 * Arguments:
 *  If no arguments are provided, the machine will home all 3 axes.
 *  You can also specify which exact axes you want to home by adding an X, Y, or Z to the command.
 */
Std_Err init_G28(GCodeCommand* cmd);

/*
 * COMMAND G90
 *  Positioning defined with reference to part zero
 *  (use absolute positioning for the XYZ axes)
 *
 *  Arguments:
 *   None
 */
Std_Err init_G90(GCodeCommand* cmd);

/*
 * COMMAND G91
 *  Positioning defined with reference to previous position
 *  (use relative positioning for the XYZ axes)
 *
 *  Arguments:
 *   None
 */
Std_Err init_G91(GCodeCommand* cmd);

/*
 * COMMAND G92
 *  This command sets the current axes position
 *
 *  Arguments:
 *   Specify the absolute coordinate for any axis that you wish to overwrite.
 *   You can include the X, Y, Z, and E axes.
 *   If you do not include one of these axes in the command, the position will remain unchanged.
 */
Std_Err init_G92(GCodeCommand* cmd);

/*
 * COMMAND M104
 *  Use these command to set the temperature of your extruder.
 *	The M104 command starts heating the extruder,
 *	 but then allows you to run other commands immediately afterwards.
 *
 *  Arguments:
 *   The S value specifies the extruder temperature in degrees Celsius.
 */
Std_Err init_M104(GCodeCommand* cmd);

/*
 * COMMAND M106
 *  This command allows you to set the speed of your printers part cooling fan.
 *  This is an external cooling fan that is pointed towards the part that you are printing.
 *
 *  Arguments:
 *   The S value sets the speed of the cooling fan in a range between 0 (off) and 255 (full power).
 */
Std_Err init_M106(GCodeCommand* cmd);

/*
 * COMMAND M109
 *  Use these command to set the temperature of your extruder.
 *  The M109 command will wait until the desired temperature is reached before allowing
 *   any other commands to run.
 *
 *  Arguments:
 *   The S value specifies the extruder temperature in degrees Celsius.
 */
Std_Err init_M109(GCodeCommand* cmd);

/*
 * COMMAND M140
 *  Use these commands to set the temperature of your heated build platform.
 *  Sending the M140 command begins heating the bed,
 *   but allows you to run other commands immediately afterwards.
 *
 *  Arguments:
 *   The S value specifies the bed temperature in degrees Celsius.
 */
Std_Err init_M140(GCodeCommand* cmd);

/*
 * COMMAND M190
 *  Use these commands to set the temperature of your heated build platform.
 *  The M190 command will wait until the bed temperature is reached before allowing
 *   any other commands to run.
 *
 *  Arguments:
 *   The S value specifies the bed temperature in degrees Celsius.
 */
Std_Err init_M190(GCodeCommand* cmd);



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* _COMMANDS_H_ */

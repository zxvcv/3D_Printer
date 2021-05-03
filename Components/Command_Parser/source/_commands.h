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

#include "Command_Parser.h"
#include <stdio.h>
#include <string.h>
#include "Project_Config.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define _OFFSET_MAXSPEED        (0)
#define _OFFSET_STEPSIZE        (_OFFSET_MAXSPEED+sizeof(double))
#define _OFFSET_POSITIONZERO    (_OFFSET_STEPSIZE+sizeof(int))
#define _OFFSET_POSITIONEND     (_OFFSET_POSITIONZERO+sizeof(int))

#define _ANGLE_MIN_VAL  (0.01)
#define _ANGLE_MAX_VAL  (45.)

/* used_fields */
#define PARAM_X     0x0001
#define PARAM_Y     0x0002
#define PARAM_Z     0x0004
#define PARAM_E     0x0008
#define PARAM_F     0x0010
#define PARAM_I     0x0020
#define PARAM_J     0x0040
#define PARAM_K     0x0080
#define PARAM_V     0x0100
#define PARAM_LAST  PARAM_V
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      PRIVATE DECLARATIONS                                    *
 * ############################################################################################ */

Std_Err forward_command_concurrently(SystemCommand_Settings* settings, SystemCommand* cmd,
    char* cmd_name, bool forward_parameters);

Std_Err forward_command_immediately(SystemCommand_Settings* settings, SystemCommand* cmd,
    char* cmd_name, bool forward_parameters);

Std_Err init_U00(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U01(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U02(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U03(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U10(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U11(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U12(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U13(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U20(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U21(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U22(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U23(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U24(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U25(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U26(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U27(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U40(SystemCommand_Settings* settings, SystemCommand* cmd);



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* _COMMANDS_H_ */

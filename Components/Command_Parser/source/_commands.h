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

/* used_fields */
#define PARAM_X     0x01
#define PARAM_Y     0x02
#define PARAM_Z     0x04
#define PARAM_E     0x08
#define PARAM_F     0x10
#define PARAM_I     0x20
#define PARAM_J     0x40
#define PARAM_K     0x80
#define PARAM_LAST  PARAM_Z
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

Std_Err step_forward_GCode(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err remove_forward_GCode(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U00(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U01(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U02(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U10(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U11(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U12(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U20(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U21(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U22(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U23(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U24(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U25(SystemCommand_Settings* settings, SystemCommand* cmd);

Std_Err init_U40(SystemCommand_Settings* settings, SystemCommand* cmd);



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* _COMMANDS_H_ */

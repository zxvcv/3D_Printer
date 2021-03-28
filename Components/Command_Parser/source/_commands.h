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
#define PARAM_LAST  PARAM_F
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

extern SystemCmdGlobal global_systemCmd_settings;
/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PRIVATE DECLARATIONS                                    *
 * ############################################################################################ */

Std_Err step_forward_GCode(SystemCommand* cmd);

Std_Err remove_forward_GCode(SystemCommand* cmd);

Std_Err init_U00(SystemCommand* cmd);

Std_Err init_U01(SystemCommand* cmd);

Std_Err init_U02(SystemCommand* cmd);

Std_Err init_U10(SystemCommand* cmd);

Std_Err init_U11(SystemCommand* cmd);

Std_Err init_U20(SystemCommand* cmd);

Std_Err init_U21(SystemCommand* cmd);

Std_Err init_U22(SystemCommand* cmd);

Std_Err init_U23(SystemCommand* cmd);

Std_Err init_U24(SystemCommand* cmd);

Std_Err init_U40(SystemCommand* cmd);



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* _COMMANDS_H_ */

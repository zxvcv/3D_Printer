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
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define ACCURACY 1000

#define _OFFSET_MAXSPEED 0
#define _OFFSET_STEPSIZE _OFFSET_MAXSPEED+sizeof(double)
#define _OFFSET_POSITIONZERO _OFFSET_STEPSIZE+sizeof(int)
#define _OFFSET_POSITIONEND _OFFSET_POSITIONZERO+sizeof(int)

#ifdef USE_INTERRUPTS
#define IRQ_ENABLE __enable_irq()
#define IRQ_DISABLE __disable_irq()
#endif /* USE_INTERRUPTS */

/* used_fields */
#define PARAM_X     0x01
#define PARAM_Y     0x02
#define PARAM_Z     0x04
#define PARAM_E     0x08
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

Std_Err init_U00(SystemCommand* cmd);

Std_Err init_U10(SystemCommand* cmd);

Std_Err init_U11(SystemCommand* cmd);

Std_Err init_U20(SystemCommand* cmd);

Std_Err init_U21(SystemCommand* cmd);

Std_Err init_U22(SystemCommand* cmd);

Std_Err init_U23(SystemCommand* cmd);

Std_Err init_U24(SystemCommand* cmd);

Std_Err init_U25(SystemCommand* cmd);

Std_Err init_U40(SystemCommand* cmd);



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* _COMMANDS_H_ */

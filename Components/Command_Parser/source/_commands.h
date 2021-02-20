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
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PRIVATE DECLARATIONS                                    *
 * ############################################################################################ */

Std_Err systemCmd_MotorDataRequest(SystemCommand* cmd, DeviceSettings* settings);

Std_Err systemCmd_MotorPositionZero(SystemCommand* cmd, DeviceSettings* settings);

Std_Err systemCmd_MotorSpeedMax(SystemCommand* cmd, DeviceSettings* settings);

Std_Err systemCmd_MotorDistanceMove(SystemCommand* cmd, DeviceSettings* settings);

Std_Err systemCmd_MotorPositionEnd(SystemCommand* cmd, DeviceSettings* settings);

Std_Err systemCmd_MotorPositionMove(SystemCommand* cmd, DeviceSettings* settings);

Std_Err systemCmd_MotorStepSizeSet(SystemCommand* cmd, DeviceSettings* settings);

Std_Err systemCmd_MotorPositionValueSet(SystemCommand* cmd, DeviceSettings* settings);

Std_Err systemCmd_SDCardProgramRun(SystemCommand* cmd, DeviceSettings* settings);

Std_Err systemCmd_MotorStepSizeRequest(SystemCommand* cmd, DeviceSettings* settings);

Std_Err systemCmd_MotorSpeedSet(SystemCommand* cmd, DeviceSettings* settings);



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* _COMMANDS_H_ */

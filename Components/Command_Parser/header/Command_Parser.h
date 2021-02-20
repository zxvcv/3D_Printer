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

#ifndef COMMAND_PARSER_H_
#define COMMAND_PARSER_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Error_Codes.h"
#include "GCode_Parser.h" //[[DEBUG]]
#include "ProjectObjects.h" //[[DEBUG]]
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define SYSTEM_COMMANDS_NUM 11
#define SYSTEM_COMMANDS_MOTORS_MAX_NUM 2
#define SYSTEM_COMMANDS_ARGS_MAX_NUM 2
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef struct SystemCommand{
    Std_Err (*execute)(struct SystemCommand*, DeviceSettings*) ;    //command pointer
    uint8_t motorsNum;
    MotorSettings *motor[SYSTEM_COMMANDS_MOTORS_MAX_NUM];
    double arg[SYSTEM_COMMANDS_ARGS_MAX_NUM];
} SystemCommand;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

Std_Err parseSystemCommand(char* cmd, SystemCommand* cmdOUT, DeviceSettings* settings);

Std_Err executeSystemCommand(SystemCommand* cmd, DeviceSettings* settings);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* COMMAND_PARSER_H_ */

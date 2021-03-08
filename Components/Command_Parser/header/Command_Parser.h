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
#include "Project_Objects.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define SYSTEM_COMMANDS_NUM 10
/*[[COMPONENT_DEFINES_H]]*/



/* ############################################################################################ *
 *                                      EXTERNS                                                 *
 * ############################################################################################ */

/*[[COMPONENT_EXTERNS_H]]*/



/* ############################################################################################ *
 *                                      DATA TYPES                                              *
 * ############################################################################################ */

typedef enum ExecutionPolicy_Tag{
    PRIORITY    = 0,
    NORMAL      = 1
}ExecutionPolicy;

typedef struct SystemCmdGlobal_Tag{
    uint8_t none;
}SystemCmdGlobal;

typedef struct SystemCommand_Tag{
    Std_Err (*init)(struct SystemCommand_Tag*);
    Std_Err (*delete)(struct SystemCommand_Tag*);

    Std_Err (*step)(struct SystemCommand_Tag*);


    ExecutionPolicy execution_policy;
    uint8_t used_fields;
    struct{
        double x;       //X-axis
        double y;       //Y-axis
        double z;       //Z-axis
        double e;       //extruder-axis
    }data;
}SystemCommand;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void init_SystemCommandsParser();

Std_Err parse_SystemCommand(char* cmd, SystemCommand* cmdOUT);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* COMMAND_PARSER_H_ */

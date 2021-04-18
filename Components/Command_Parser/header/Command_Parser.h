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
#include "Buffered_Communication.h"
#include "GCode_Parser.h"
#include "Manager_EEPROM.h"
#include "Manager_SDcard.h"
/*[[COMPONENT_INCLUDES_H]]*/



/* ############################################################################################ *
 *                                      DEFINES                                                 *
 * ############################################################################################ */

#define COMM_MSG_BUFF_SIZE 100
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

typedef struct SystemCommand_Settings_Tag{
    BuffCommunication_Settings* buff_comm;
    Motor** motors;
    EEPROMSettings* eeprom;
    SDCard_Settings* sd;

    uint8_t* motor_data_addresses;

    char msg_buff[COMM_MSG_BUFF_SIZE];
}SystemCommand_Settings;

typedef struct SystemCommand_Tag{
    Std_Err (*init)(SystemCommand_Settings*, struct SystemCommand_Tag*);
    Std_Err (*remove)(SystemCommand_Settings*, struct SystemCommand_Tag*);

    Std_Err (*step)(SystemCommand_Settings*, struct SystemCommand_Tag*);


    ExecutionPolicy execution_policy;
    uint8_t used_fields;
    struct{
        double x;       //X-axis
        double y;       //Y-axis
        double z;       //Z-axis
        double e;       //extruder-axis
        double f;       //speed of the movement
    }data;

    GCodeCommand gcode_cmd;
}SystemCommand;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void init_SystemCommandsParser(SystemCommand_Settings* settings,
    BuffCommunication_Settings* buff_comm, Motor** motors, EEPROMSettings* eeprom,
    SDCard_Settings* sd, uint8_t* motor_data_addresses);

Std_Err parse_SystemCommand(SystemCommand_Settings* settings, char* cmd, SystemCommand* cmdOUT);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* COMMAND_PARSER_H_ */

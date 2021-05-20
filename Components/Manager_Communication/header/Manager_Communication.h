/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Manager_Communication
 *      [[COMPONENT_DESCRIPTION]]
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef MANAGER_COMMUNICATION_H_
#define MANAGER_COMMUNICATION_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include "Error_Codes.h"
#include "Buffered_Communication.h"
#include "Command_Parser.h"
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

typedef struct Communication_Settings_Tag{
    BuffCommunication_Settings* buff_comm;
    SystemCommand_Settings sys_comm;

    SystemCommand executingCmd;

    struct{
        bool executing_command;
    }flags;
}Communication_Settings;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

void init_communication_manager(Communication_Settings* settings,
    BuffCommunication_Settings* buff_comm, Motor** motors, EEPROMSettings* eeprom,
    SDCard_Settings* sd, BoundariesDetector_Settings* boundaryDetection,
    uint8_t* motor_data_addresses);

Std_Err parse_communication_command(Communication_Settings* settings);

Std_Err execute_communication_command(Communication_Settings* settings);

Std_Err send_communication_command(Communication_Settings* settings);

Std_Err send_message(Communication_Settings* settings, char* msg, uint8_t msgSize);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* MANAGER_COMMUNICATION_H_ */

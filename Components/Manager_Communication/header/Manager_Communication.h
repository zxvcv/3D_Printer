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

#include "Buffered_Communication.h"
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

typedef struct Communication_Flags_Tag{
    // bool eofRecieved;
    // bool end_program;
    bool executing_program;
    bool executing_command;
}Communication_Flags;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

Std_Err init_communication_manager(BuffCommunication_Settings* settings, UART_HandleTypeDef* huart);

Std_Err parse_communication_command(BuffCommunication_Settings* settings);

Std_Err execute_communication_command(BuffCommunication_Settings* settings, bool motors_state);

Std_Err send_communication_command(BuffCommunication_Settings* settings);

Std_Err send_message(BuffCommunication_Settings* settings, char* msg, uint8_t msgSize);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* MANAGER_COMMUNICATION_H_ */

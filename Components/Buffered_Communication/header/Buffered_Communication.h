/* ############################################################################################ *
 *                                LICENSE INFORMATIONS                                          *
 * ============================================================================================
 * Copyright (c) 2020-2021 Paweł Piskorz
 * Licensed under the Eclipse Public License 2.0
 * See attached LICENSE file
 * ############################################################################################ */
/************************************************************************************************
 * NAME: Buffered_Communication
 *      [[COMPONENT_DESCRIPTION]]
 * ============================================================================================
 * COMMENTS:
 *      [[COMPONENT_COMMENTS]]
 * ============================================================================================
 * EXAMPLE:
 *      [[COMPONENT_EXAMPLE]]
 ************************************************************************************************/

#ifndef BUFFERED_COMMUNICATION_H_
#define BUFFERED_COMMUNICATION_H_


/* ############################################################################################ *
 *                                      INCLUDES                                                *
 * ############################################################################################ */

#include <stdbool.h>
#include "FIFO_void.h"
#include "Error_Codes.h"
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

typedef struct BuffCommunication_Settings_Tag{
    Fifo_C* Buff_InputCommands;
    Fifo_C* Buff_IN;
    Fifo_C* Buff_OUT;

    UART_HandleTypeDef* huart;

    uint8_t recieved;
    bool EOL_recieved;
    bool transmission;
}BuffCommunication_Settings;
/*[[COMPONENT_DATA_TYPES_H]]*/



/* ############################################################################################ *
 *                                      PUBLIC DECLARATIONS                                     *
 * ############################################################################################ */

Std_Err init_buffered_communication(BuffCommunication_Settings* settings, UART_HandleTypeDef* huart);

Std_Err send_buffered_message(BuffCommunication_Settings* settings);

Std_Err send_buffered_message_IT(BuffCommunication_Settings* settings);

Std_Err receive_buffered_message_IT(BuffCommunication_Settings* settings);

Std_Err deinit_buffered_communication(BuffCommunication_Settings* settings);
/*[[COMPONENT_PUBLIC_DECLARATIONS]]*/



#endif /* BUFFERED_COMMUNICATION_H_ */

/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 *
 * =======================================================================================================
 * COMMENTS:
 * 		.
 * =======================================================================================================
 * EXAMPLE:
 *
 ********************************************************************************************************** */

#ifndef MANAGER_BT_H_
#define MANAGER_BT_H_



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include <stdbool.h>
#include "FIFO_void.h"
#include "main.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */

typedef struct BT_Settings_Tag{
	List* Buff_InputCommandsBT;
	List* Buff_Bt_IN;
	List* Buff_Bt_OUT;

	UART_HandleTypeDef* huart;

	uint8_t recievedBT;
	bool EOL_BT_recieved;
	bool transmissionBT;
}BT_Settings;



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DECLARATIONS
 * ####################################################################################################### */

void init_operations_BT(BT_Settings* settings);
void execute_command_BT(BT_Settings* settings);
void parse_data_BT(BT_Settings* settings);
void send_command_BT(BT_Settings* settings);



#endif /*MANAGER_BT_H_*/

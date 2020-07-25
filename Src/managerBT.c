/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 *
 * =======================================================================================================
 * COMMENTS:
 *
 * =======================================================================================================
 * EXAMPLE:
 *
 ********************************************************************************************************** */



/* #######################################################################################################
 *											INCLUDES
 * ####################################################################################################### */

#include "managerBT.h"
#include "parserCommand.h"
#include "manager.h"


/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */

extern DeviceSettings printerSettings; /* TO DO delete this extern */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

Std_Err init_operations_BT(BT_Settings* settings){
	Std_Err retVal = STD_OK;
	Fifo_Err fifoErr;
	HAL_StatusTypeDef halStatus;

	list_create(&(settings->Buff_InputCommandsBT), &fifoErr);
	if(fifoErr != QUEUE_OK)
	{
		return translate_error_fifo_to_project(fifoErr);
	}

	list_create(&(settings->Buff_Bt_IN), &fifoErr);
	if(fifoErr != QUEUE_OK)
	{
		return translate_error_fifo_to_project(fifoErr);
	}

	list_create(&(settings->Buff_Bt_OUT), &fifoErr);
	if(fifoErr != QUEUE_OK)
	{
		return translate_error_fifo_to_project(fifoErr);
	}

	halStatus = HAL_UART_Receive_IT(settings->huart, &(settings->recievedBT), 1);
	if(halStatus != HAL_OK)
	{
		return translate_error_hal_to_project(halStatus);
	}

	return retVal;
}

void execute_command_BT(BT_Settings* settings){
	Fifo_Err errors;

	if(list_getSize(settings->Buff_InputCommandsBT, &errors) > 0){
		executeSystemCommand((SystemCommand*)list_front(settings->Buff_InputCommandsBT, &errors), &printerSettings);
		list_pop_C(settings->Buff_InputCommandsBT, &errors);
	}
}

void parse_data_BT(BT_Settings* settings){
	Fifo_Err errors;

	if(settings->EOL_BT_recieved){
		settings->EOL_BT_recieved = false;
		uint8_t sizeTemp = 0;
		uint8_t temp[25];
		do{
			temp[sizeTemp++] = *((uint8_t*)list_front(settings->Buff_Bt_IN, &errors));
			list_pop_C(settings->Buff_Bt_IN, &errors);
		}while(temp[sizeTemp - 1] != '\n');
		temp[sizeTemp - 1] = '\0';

		SystemCommand cmd;
		parseSystemCommand((char*)temp, &cmd, &printerSettings);


		//__disable_irq();
		//List_Push_C(Buff_InputCommandsBT, &cmd, sizeof(SystemCommand));
		//__enable_irq();
	  }
}

void send_command_BT(BT_Settings* settings){
	Fifo_Err errors;

	if(!settings->transmissionBT && list_getSize(settings->Buff_Bt_OUT, &errors) > 0){
		settings->transmissionBT = true;
		HAL_UART_Transmit_IT(settings->huart, (uint8_t*)list_front(settings->Buff_Bt_OUT, &errors), list_getDataSize(settings->Buff_Bt_OUT, &errors));
	}
}

Std_Err deinit_operations_BT(BT_Settings* settings)
{
	Std_Err retVal = STD_OK;
	Fifo_Err fifoErr;
	HAL_StatusTypeDef halStatus;

	list_delete_C(&(settings->Buff_InputCommandsBT), &fifoErr);
	if(fifoErr != QUEUE_OK)
	{
		return translate_error_fifo_to_project(fifoErr);
	}

	list_delete_C(&(settings->Buff_Bt_IN), &fifoErr);
	if(fifoErr != QUEUE_OK)
	{
		return translate_error_fifo_to_project(fifoErr);
	}

	list_delete_C(&(settings->Buff_Bt_OUT), &fifoErr);
	if(fifoErr != QUEUE_OK)
	{
		return translate_error_fifo_to_project(fifoErr);
	}

	return retVal;
}

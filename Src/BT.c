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

#include "BT.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

Std_Err init_operations_BT(BT_Settings* settings)
{
	Std_Err stdErr = STD_OK;
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

	return stdErr;
}

Std_Err send_command_BT(BT_Settings* settings)
{
	Std_Err stdErr = STD_OK;
	Fifo_Err fifoErr;
	HAL_StatusTypeDef halErr;
	uint8_t* data;
	uint8_t dataSize;

	uint8_t listSize = list_getSize(settings->Buff_Bt_OUT, &fifoErr);
	if(fifoErr != QUEUE_OK)
	{
		return translate_error_fifo_to_project(fifoErr);
	}

	if(!settings->transmissionBT && listSize > 0)
	{
		settings->transmissionBT = true;
		data = (uint8_t*)list_front(settings->Buff_Bt_OUT, &fifoErr);
		if(fifoErr != QUEUE_OK)
		{
			return translate_error_fifo_to_project(fifoErr);
		}
		
		dataSize = list_getDataSize(settings->Buff_Bt_OUT, &fifoErr);
		if(fifoErr != QUEUE_OK)
		{
			return translate_error_fifo_to_project(fifoErr);
		}

		halErr = HAL_UART_Transmit_IT(settings->huart, data, dataSize);

		stdErr = translate_error_hal_to_project(halErr);
	}
	else if(settings->transmissionBT)
	{
		stdErr = STD_BUSY_ERROR;
	}	

	return stdErr;
}

Std_Err deinit_operations_BT(BT_Settings* settings)
{
	Std_Err stdErr = STD_OK;
	Fifo_Err fifoErr;

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

	return stdErr;
}

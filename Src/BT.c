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
	HAL_StatusTypeDef halStatus;

	stdErr = fifo_create(&(settings->Buff_InputCommandsBT));
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	stdErr = fifo_create(&(settings->Buff_Bt_IN));
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	stdErr = fifo_create(&(settings->Buff_Bt_OUT));
	if(stdErr != STD_OK)
	{
		return stdErr;
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
	HAL_StatusTypeDef halErr;
	uint8_t* data = NULL;
	uint8_t dataSize;

	uint8_t listSize = fifo_getSize(settings->Buff_Bt_OUT);

	if(!settings->transmissionBT && listSize > 0)
	{
		settings->transmissionBT = true;
		stdErr = fifo_front(settings->Buff_Bt_OUT, (void**)&data);
		if(stdErr != STD_OK)
		{
			return stdErr;
		}
		
		dataSize = fifo_getDataSize(settings->Buff_Bt_OUT);

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

	stdErr = fifo_delete_C(&(settings->Buff_InputCommandsBT));
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	stdErr = fifo_delete_C(&(settings->Buff_Bt_IN));
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	stdErr = fifo_delete_C(&(settings->Buff_Bt_OUT));
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	return stdErr;
}

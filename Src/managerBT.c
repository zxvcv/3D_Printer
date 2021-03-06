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



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

Std_Err execute_command_BT(DeviceSettings* settings)
{
	Std_Err stdErr = STD_OK;
	Fifo_Err fifoErr;

	SystemCommand* cmd;
	uint8_t listSize = list_getSize(settings->bt->Buff_InputCommandsBT, &fifoErr);

	if(fifoErr != QUEUE_OK)
	{
		stdErr = translate_error_fifo_to_project(fifoErr);
		return stdErr;
	}

	if(listSize > 0)
	{
		cmd = (SystemCommand*)list_front(settings->bt->Buff_InputCommandsBT, &fifoErr);
		if(fifoErr != QUEUE_OK)
		{
			return translate_error_fifo_to_project(fifoErr);
		}

		stdErr = executeSystemCommand(cmd, settings);

		if(stdErr != STD_OK)
		{
			return stdErr;
		}

		list_pop_C(settings->bt->Buff_InputCommandsBT, &fifoErr);
		stdErr = translate_error_fifo_to_project(fifoErr);
	}
	else
	{
		stdErr = STD_OK;
	}
	
	return stdErr;
}

Std_Err parse_data_BT(DeviceSettings* settings)
{
	Std_Err stdErr = STD_OK;
	Fifo_Err fifoErr;

	SystemCommand cmd;
	uint8_t sizeTemp = 0;
	uint8_t temp[25];

	if(settings->EOL_BT_recieved)
	{
		settings->EOL_BT_recieved = false;

		do
		{
			temp[sizeTemp++] = *((uint8_t*)list_front(settings->bt->Buff_Bt_IN, &fifoErr));
			if(fifoErr != QUEUE_OK)
			{
				return translate_error_fifo_to_project(fifoErr);
			}
			
			list_pop_C(settings->bt->Buff_Bt_IN, &fifoErr);
			if(fifoErr != QUEUE_OK)
			{
				return translate_error_fifo_to_project(fifoErr);
			}

		}while(temp[sizeTemp - 1] != '\n');
		temp[sizeTemp - 1] = '\0';

		stdErr = parseSystemCommand((char*)temp, &cmd, settings);
		
#ifdef USE_INTERRUPTS
		IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

		list_push_C(settings->bt->Buff_InputCommandsBT, &cmd, sizeof(SystemCommand), &fifoErr);

#ifdef USE_INTERRUPTS
		IRQ_ENABLE;
#endif /* USE_INTERRUPTS */
		if(fifoErr != QUEUE_OK)
		{
			return translate_error_fifo_to_project(fifoErr);
		}

	}

	return stdErr;
}


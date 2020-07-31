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

	SystemCommand* cmd = NULL;
	uint8_t listSize;

	listSize = fifo_getSize(settings->bt->Buff_InputCommandsBT);

	if(listSize > 0)
	{
		stdErr = fifo_front(settings->bt->Buff_InputCommandsBT, (void**)&cmd);
		if(stdErr != STD_OK)
		{
			return stdErr;
		}

		stdErr = executeSystemCommand(cmd, settings);

		if(stdErr != STD_OK)
		{
			return stdErr;
		}

		stdErr = fifo_pop_C(settings->bt->Buff_InputCommandsBT);
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

	SystemCommand cmd;
	uint8_t sizeTemp = 0;
	uint8_t* data;
	uint8_t temp[25];

	if(settings->EOL_BT_recieved)
	{
		settings->EOL_BT_recieved = false;

		do
		{
			stdErr = fifo_front(settings->bt->Buff_Bt_IN, (void*)&data);
			temp[sizeTemp++] = *data;
			if(stdErr != STD_OK)
			{
				return stdErr;
			}
			
			stdErr = fifo_pop_C(settings->bt->Buff_Bt_IN);
			if(stdErr != STD_OK)
			{
				return stdErr;
			}

		}while(temp[sizeTemp - 1] != '\n');
		temp[sizeTemp - 1] = '\0';

		stdErr = parseSystemCommand((char*)temp, &cmd, settings);
		
#ifdef USE_INTERRUPTS
		IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

		stdErr = fifo_push_C(settings->bt->Buff_InputCommandsBT, &cmd, sizeof(SystemCommand));

#ifdef USE_INTERRUPTS
		IRQ_ENABLE;
#endif /* USE_INTERRUPTS */
	}

	return stdErr;
}


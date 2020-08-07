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

#include "managerOuterComm.h"
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

Std_Err execute_outer_command(DeviceSettings* settings)
{
	Std_Err stdErr = STD_OK;

	SystemCommand* cmd = NULL;
	uint8_t listSize;

	listSize = fifo_getSize(settings->outComm->Buff_InputCommands);

	if(listSize > 0)
	{
		stdErr = fifo_front(settings->outComm->Buff_InputCommands, (void**)&cmd);
		if(stdErr != STD_OK)
		{
			return stdErr;
		}

		stdErr = executeSystemCommand(cmd, settings);

		if(stdErr != STD_OK)
		{
			return stdErr;
		}

		stdErr = fifo_pop_C(settings->outComm->Buff_InputCommands);
	}
	else
	{
		stdErr = STD_OK;
	}
	
	return stdErr;
}

Std_Err parse_outer_data(DeviceSettings* settings)
{
	Std_Err stdErr = STD_OK;

	SystemCommand cmd;
	uint8_t sizeTemp = 0;
	uint8_t* data;
	uint8_t temp[25];

	if(settings->EOL_recieved)
	{
		settings->EOL_recieved = false;

		do
		{
			stdErr = fifo_front(settings->outComm->Buff_IN, (void*)&data);
			temp[sizeTemp++] = *data;
			if(stdErr != STD_OK)
			{
				return stdErr;
			}
			
			stdErr = fifo_pop_C(settings->outComm->Buff_IN);
			if(stdErr != STD_OK)
			{
				return stdErr;
			}

		}while(temp[sizeTemp - 1] != '\n');
		temp[sizeTemp - 1] = '\0';

		stdErr = parseSystemCommand((char*)temp, &cmd, settings);
		if(stdErr != STD_OK)
		{
			return stdErr;
		}
#ifdef USE_INTERRUPTS
		IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

		stdErr = fifo_push_C(settings->outComm->Buff_InputCommands, &cmd, sizeof(SystemCommand));

#ifdef USE_INTERRUPTS
		IRQ_ENABLE;
#endif /* USE_INTERRUPTS */
	}

	return stdErr;
}


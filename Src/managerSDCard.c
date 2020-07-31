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

#include "managerSDCard.h"
#include <string.h>
#include "manager.h"
#include "parserGCode.h"


/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *										PRIVATE DATA
 * ####################################################################################################### */

GCodeCommand executingCmd; /*TODO: delete this */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

/*TODO: rework LOG_ENABLE mode*/

Std_Err init_operations_SDcard(SDCard_Settings* settings)
{
	Std_Err stdErr = STD_OK;

	settings->end_SDprogram = false;
	settings->eofRecieved = false;
	settings->executing_SDprogram = false;
	settings->executing_SDcommand = false;

	settings->activeTab = 1;
	settings->unactiveTab = 0;
	settings->counterTab[0] = BYTES_TO_READ;
	settings->counterTab[1] = BYTES_TO_READ;
	settings->cnt = 0;
	settings->bytesRead = 0;

	stdErr = fifo_create(&(settings->BuffIN_SDcmd));
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

#ifdef LOG_ENABLE
	stdErr = fifo_create(&(settings->BuffOUT_logs));
#endif /*LOG_ENABLE*/

	return stdErr;
}

Std_Err parse_data_SDcard(SDCard_Settings* settings)
{
	Std_Err stdErr = STD_OK;
	
	uint8_t listSize;

	listSize = fifo_getSize(settings->BuffIN_SDcmd);

	while(!settings->end_SDprogram && listSize < 5 && settings->executing_SDprogram)
	{
		if(settings->cnt >= settings->bytesRead)
		{
			if(settings->eofRecieved)
			{ 
				settings->end_SDprogram = true;
				/*TODO: check if this return value is correct*/
				return STD_OK; 
			}

			settings->activeTab = settings->activeTab ? 0 : 1;
			settings->unactiveTab = settings->activeTab ? 0 : 1;

			f_read(settings->file, settings->dataSDin[settings->activeTab], BYTES_TO_READ, 
				   &(settings->bytesRead));

			if(BYTES_TO_READ > settings->bytesRead)
			{
				settings->eofRecieved = true;
			}

			settings->counterTab[settings->activeTab] = 0;
			settings->cnt = 0;
		}

		while(settings->cnt < settings->bytesRead && 
			  settings->dataSDin[settings->activeTab][settings->cnt] != '\n')
		{
			++(settings->cnt);
		}

		if(settings->dataSDin[settings->activeTab][settings->cnt] == '\n')
		{
			uint8_t cmdData[BYTES_TO_READ];
			uint8_t cmdLen = 0;

			if(settings->counterTab[settings->unactiveTab] < BYTES_TO_READ)
			{
				cmdLen = BYTES_TO_READ - settings->counterTab[settings->unactiveTab];

				memcpy(	cmdData, 
						settings->dataSDin[settings->unactiveTab] + settings->counterTab[settings->unactiveTab], 
						cmdLen);

				settings->counterTab[settings->unactiveTab] += cmdLen;
			}

			memcpy(	cmdData + cmdLen, 
					settings->dataSDin[settings->activeTab] + settings->counterTab[settings->activeTab], 
					settings->cnt - 1);
			cmdLen += settings->cnt - settings->counterTab[settings->activeTab];
			settings->counterTab[settings->activeTab] = settings->cnt + 1;
			
			/*TODO: make this function work with \r\n (Windows) and \n (Linux)*/
			/*TODO: check below comment*/
			/*cmdData[cmdLen - 2] = '\0'; powinno usunac  ostatni znak w komendzie ale pewny nie jestem*/
			cmdData[cmdLen - 1] = '\0'; /*change '\n' to '\0'*/
			//cmdData[cmdLen] = '\0';

			GCodeCommand cmd;
			stdErr = parseGCodeCommand((char*)cmdData, &cmd);
			if(stdErr != STD_OK)
			{
				return stdErr;
			}

			stdErr = fifo_push_C(settings->BuffIN_SDcmd, &cmd, sizeof(GCodeCommand));
			if(stdErr != STD_OK)
			{
				return stdErr;
			}

			++(settings->cnt);
		}

		listSize = fifo_getSize(settings->BuffIN_SDcmd);
	}

	return stdErr;
}

Std_Err execute_command_SDcard(DeviceSettings* settings)
{
	Std_Err stdErr = STD_OK;

	GCodeCommand* cmd = NULL;
	uint8_t fifoSize;

	fifoSize = fifo_getSize(settings->sd->BuffIN_SDcmd);

	if(settings->sd->executing_SDprogram && 
		fifoSize > 0 && 
		!settings->sd->executing_SDcommand)
	{
		stdErr = fifo_front(settings->sd->BuffIN_SDcmd, (void**)&cmd);
		if(stdErr != STD_OK)
		{
			return stdErr;
		}

		memcpy(&(executingCmd), cmd, sizeof(GCodeCommand));
		stdErr = fifo_pop_C(settings->sd->BuffIN_SDcmd);
		if(stdErr != STD_OK)
		{
			return stdErr;
		}

#ifdef LOG_ENABLE
		uint8_t data[100], size = 0;
		size += sprintf(data, "[CMD%d] ", executingCmd.cmdNum); //command type
		if(executingCmd.usedFields._x) size += sprintf(data + size, " X% 10.5f,", executingCmd.x);
		if(executingCmd.usedFields._y) size += sprintf(data + size, " Y% 10.5f,", executingCmd.y);
		if(executingCmd.usedFields._z) size += sprintf(data + size, " Z% 10.5f,", executingCmd.z);
		if(executingCmd.usedFields._e) size += sprintf(data + size, " E% 10.5f,", executingCmd.e);
		if(executingCmd.usedFields._f) size += sprintf(data + size, " F% 10.5f,", executingCmd.f);
		if(executingCmd.usedFields._s) size += sprintf(data + size, " S% 10.5f,", executingCmd.s);
		data[--size] = '\r'; data[++size] = '\n'; ++size;
		List_Push_C(BuffOUT_logs, data, size);
		size = sprintf(data, "$MovErr: % 10d, % 10d, % 10d, % 10d\r\n",
				motors[0].data.err.roundingMoveError, motors[1].data.err.roundingMoveError,
				motors[2].data.err.roundingMoveError, motors[3].data.err.roundingMoveError);
		List_Push_C(BuffOUT_logs, data, size);
		size = sprintf(data, "$PosBef: %10d, %10d, %10d, %10d\r\n",
				motors[0].data.position, motors[1].data.position, motors[2].data.position, motors[3].data.position);
		List_Push_C(BuffOUT_logs, data, size);
#endif /*LOG_ENABLE*/

		stdErr = executeGCodeCommand(&(executingCmd), settings);
	}

	if(settings->errMove)
	{
		settings->errMove = false;
		settings->sd->end_SDprogram = false;
		settings->sd->executing_SDprogram = false;
		settings->sd->executing_SDcommand = false;

		settings->sd->activeTab = 1;
		settings->sd->counterTab[0] = BYTES_TO_READ;
		settings->sd->counterTab[1] = BYTES_TO_READ;

		/*TODO: error handling for while below*/
		while(fifo_getSize(settings->sd->BuffIN_SDcmd) > 0)
		{
			stdErr = fifo_pop_C(settings->sd->BuffIN_SDcmd);
			if(stdErr != STD_OK)
			{
				return stdErr;
			}
		}

		f_close(settings->sd->file);

		/*TODO: error handling for SDCard librabry*/

#ifdef LOG_ENABLE
		uint8_t data[100], size = 0;
		size = sprintf(data, "[ERR]ERROR OCCURED WHILE MOVING HEAD\r\n[STOP]\r\n");
		List_Push_C(BuffOUT_logs, data, size);
#endif /*LOG_ENABLE*/

		stdErr = STD_ERROR;
	}

	return stdErr;
}

#ifdef LOG_ENABLE
Std_Err send_logs_SDcard()
{
	if(List_GetSize(BuffOUT_logs) > 0)
	{
		UINT bytesWritten;
		f_write(&logFile, List_Front(BuffOUT_logs), List_GetDataSize(BuffOUT_logs), &bytesWritten);
		f_sync(&logFile); // could be done once per E.g. 10s, not every time
		List_Pop_C(BuffOUT_logs);
	}
}
#endif /*LOG_ENABLE*/

Std_Err reset_commands_SDcard(SDCard_Settings* settings)
{
	Std_Err stdErr = STD_OK;
	uint8_t listSize;

	listSize = fifo_getSize(settings->BuffIN_SDcmd);

	if(	settings->end_SDprogram && 
		settings->executing_SDprogram && 
		!settings->executing_SDcommand && 
		listSize == 0)
	{
		settings->end_SDprogram = false;
		settings->executing_SDprogram = false;
		f_close(settings->file);

		/*TODO: error handling for SDCard librabry*/

#ifdef LOG_ENABLE
		uint8_t data[100], size = 0;
		size = sprintf(data, "[STOP]\r\n"); //command type
		size += sprintf(data + size, "$PosBef: %10d, %10d, %10d, %10d\r\n",
				motors[0].data.position, motors[1].data.position, motors[2].data.position, motors[3].data.position);
		List_Push_C(BuffOUT_logs, data, size);

		while(List_GetSize(BuffOUT_logs) > 0)
			send_logs_SDcard();
		f_close(&logFile);
#endif /*LOG_ENABLE*/
	}

	return stdErr;
}

Std_Err detecting_endCommand_SDcard(DeviceSettings* settings)
{
	Std_Err stdErr = STD_OK;

	bool flag;

	if(settings->sd->executing_SDcommand)
	{
		flag = false;

		for(int i=0; i<MOTORS_NUM; ++i)
		{
			flag |= settings->motors[i]->flags.isOn;
		}
			
		if(!flag)
		{
			settings->sd->executing_SDcommand = false;
		}
	}
	else
	{
		stdErr = STD_BUSY_ERROR;
	}
	

	return stdErr;
}


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

extern DeviceSettings printerSettings; /* TO DO delete this extern */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

void init_operations_SDcard(SDCard_Settings* settings){
	Fifo_Err errors;

	settings->end_SDprogram = false;
	settings->executing_SDprogram = false;
	settings->executing_SDcommand = false;

	settings->activeTab = 1;
	settings->counterTab[0] = BYTES_TO_READ;
	settings->counterTab[1] = BYTES_TO_READ;

	list_create(&(settings->BuffIN_SDcmd), &errors);
#ifdef LOG_ENABLE
	list_create(settings->BuffOUT_logs, &errors);
#endif
}


bool eofRecieved = false;
UINT bytesRead = 0;
uint8_t unactiveTab, cnt = 0;
void parse_data_SDcard(SDCard_Settings* settings){
	Fifo_Err errors;

	while(!settings->end_SDprogram && list_getSize(settings->BuffIN_SDcmd, &errors)<5 && settings->executing_SDprogram){
		if(cnt >= bytesRead){
			if(eofRecieved){ settings->end_SDprogram = true; return; }

			settings->activeTab = settings->activeTab ? 0 : 1;
			unactiveTab = settings->activeTab ? 0 : 1;
			f_read(settings->file, settings->dataSDin[settings->activeTab], BYTES_TO_READ, &bytesRead);
			if(BYTES_TO_READ > bytesRead) eofRecieved = true;
			settings->counterTab[settings->activeTab] = 0;
			cnt = 0;
		}

		while(cnt < bytesRead && settings->dataSDin[settings->activeTab][cnt] != '\n') ++cnt;

		if(settings->dataSDin[settings->activeTab][cnt] == '\n'){
			uint8_t cmdData[BYTES_TO_READ];
			uint8_t cmdLen = 0;
			if(settings->counterTab[unactiveTab] < 50){
				cmdLen = BYTES_TO_READ - settings->counterTab[unactiveTab];
				memcpy(cmdData, settings->dataSDin[unactiveTab] + settings->counterTab[unactiveTab], cmdLen);
				settings->counterTab[unactiveTab] += cmdLen;
			}
			memcpy(cmdData + cmdLen, settings->dataSDin[settings->activeTab] + settings->counterTab[settings->activeTab], cnt - 1);
			cmdLen += cnt - settings->counterTab[settings->activeTab];
			settings->counterTab[settings->activeTab] = cnt + 1;
			cmdData[cmdLen - 1] = '\0'; // cmdData[cmdLen - 2] = '\0'; powinno usunac  ostatni znak w komendzie ale pewny nie jestem

			GCodeCommand cmd;
			parseGCodeCommand((char*)cmdData, &cmd);
			list_push_C(settings->BuffIN_SDcmd, &cmd, sizeof(GCodeCommand), &errors);

			++cnt;
		}
	}
}


GCodeCommand executingCmd;
void execute_command_SDcard(SDCard_Settings* settings){
	Fifo_Err errors;

	if(settings->executing_SDprogram && list_getSize(settings->BuffIN_SDcmd, &errors) > 0 && !settings->executing_SDcommand){
		memcpy(&executingCmd, list_front(settings->BuffIN_SDcmd, &errors), sizeof(GCodeCommand));
		list_pop_C(settings->BuffIN_SDcmd, &errors);
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
#endif
		executeGCodeCommand(&executingCmd);
	}

	if(printerSettings.errMove){
		printerSettings.errMove = false;
		settings->end_SDprogram = false;
		settings->executing_SDprogram = false;
		settings->executing_SDcommand = false;

		settings->activeTab = 1;
		settings->counterTab[0] = BYTES_TO_READ;
		settings->counterTab[1] = BYTES_TO_READ;

		while(list_getSize(settings->BuffIN_SDcmd, &errors) > 0)
			list_pop_C(settings->BuffIN_SDcmd, &errors);

		f_close(settings->file);
#ifdef LOG_ENABLE
		uint8_t data[100], size = 0;
		size = sprintf(data, "[ERR]ERROR OCCURED WHILE MOVING HEAD\r\n[STOP]\r\n");
		List_Push_C(BuffOUT_logs, data, size);
#endif
	}
}


#ifdef LOG_ENABLE
void send_logs_SDcard(){
	if(List_GetSize(BuffOUT_logs) > 0){
		UINT bytesWritten;
		f_write(&logFile, List_Front(BuffOUT_logs), List_GetDataSize(BuffOUT_logs), &bytesWritten);
		f_sync(&logFile); // could be done once per E.g. 10s, not every time
		List_Pop_C(BuffOUT_logs);
	}
}
#endif


void reset_commands_SDcard(SDCard_Settings* settings){
	Fifo_Err errors;

	if(settings->end_SDprogram && settings->executing_SDprogram && !settings->executing_SDcommand && list_getSize(settings->BuffIN_SDcmd, &errors) == 0){
		settings->end_SDprogram = false;
		settings->executing_SDprogram = false;
		f_close(settings->file);
#ifdef LOG_ENABLE
		uint8_t data[100], size = 0;
		size = sprintf(data, "[STOP]\r\n"); //command type
		size += sprintf(data + size, "$PosBef: %10d, %10d, %10d, %10d\r\n",
				motors[0].data.position, motors[1].data.position, motors[2].data.position, motors[3].data.position);
		List_Push_C(BuffOUT_logs, data, size);

		while(List_GetSize(BuffOUT_logs) > 0)
			send_logs_SDcard();
		f_close(&logFile);
#endif
	}
}

void detecting_endCommand_SDcard(SDCard_Settings* settings){
	if(settings->executing_SDcommand){
		bool flag = false;
		for(int i=0; i<MOTORS_NUM; ++i)
			flag |= printerSettings.motors[i]->flags.isOn;
		if(!flag)
			settings->executing_SDcommand = false;
	}
}


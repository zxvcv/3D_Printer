#include "manager.h"

#include <stdbool.h>
#include "FIFO_void.h"
#include "parserGCode.h"
#include "parserCommand.h"
#include "../Drivers/FATFS/ff.h"


/********** Handling Bluetooth System Commands **********/
List* Buff_InputCommandsBT = NULL;
List* Buff_Bt_IN = NULL;
List* Buff_Bt_OUT = NULL;
extern uint8_t recievedBT;
extern bool EOL_BT_recieved;
extern bool transmissionBT;
extern UART_HandleTypeDef huart1;


void init_operations_BT(){
	List_Create(&Buff_InputCommandsBT);
	List_Create(&Buff_Bt_IN);
	List_Create(&Buff_Bt_OUT);

	HAL_UART_Receive_IT(&huart1, &recievedBT, 1);
}


void execute_command_BT(){
	if(List_GetSize(Buff_InputCommandsBT) > 0){
		executeSystemCommand((SystemCommand*)List_Front(Buff_InputCommandsBT));
		List_Pop_C(Buff_InputCommandsBT);
	}
}


void parse_data_BT(){
	if(EOL_BT_recieved){
		EOL_BT_recieved = false;
		uint8_t sizeTemp = 0;
		uint8_t temp[25];
		do{
			temp[sizeTemp++] = *((uint8_t*)List_Front(Buff_Bt_IN));
			List_Pop_C(Buff_Bt_IN);
		}while(temp[sizeTemp - 1] != '\n');
		temp[sizeTemp - 1] = '\0';

		SystemCommand cmd;
		parseSystemCommand((char*)temp, &cmd);

		__disable_irq();
		List_Push_C(Buff_InputCommandsBT, &cmd, sizeof(SystemCommand));
		__enable_irq();
	  }
}


void send_command_BT(){
	if(!transmissionBT && List_GetSize(Buff_Bt_OUT) > 0){
		transmissionBT = true;
		HAL_UART_Transmit_IT(&huart1, (uint8_t*)List_Front(Buff_Bt_OUT), List_GetDataSize(Buff_Bt_OUT));
	}
}



/********** Handling SD_Card Commands **********/

#define BYTES_TO_READ 50

bool end_SDprogram, executing_SDprogram, executing_SDcommand;
FIL file;
List* BuffIN_SDcmd = NULL;
uint8_t activeTab;
uint8_t dataSDin[2][BYTES_TO_READ];
uint8_t counterTab[2];
#ifdef LOG_ENABLE
FIL logFile;
List* BuffOUT_logs = NULL;
#endif


void init_operations_SDcard(){
	end_SDprogram = false;
	executing_SDprogram = false;
	executing_SDcommand = false;

	activeTab = 1;
	counterTab[0] = BYTES_TO_READ;
	counterTab[1] = BYTES_TO_READ;

	List_Create(&BuffIN_SDcmd);
#ifdef LOG_ENABLE
	List_Create(&BuffOUT_logs);
#endif
}


bool eofRecieved = false;
UINT bytesRead = 0;
uint8_t unactiveTab, cnt = 0;
void parse_data_SDcard(){
	while(!end_SDprogram && List_GetSize(BuffIN_SDcmd)<5 && executing_SDprogram){
		if(cnt >= bytesRead){
			if(eofRecieved){ end_SDprogram = true; return; }

			activeTab = activeTab ? 0 : 1;
			unactiveTab = activeTab ? 0 : 1;
			f_read(&file, dataSDin[activeTab], BYTES_TO_READ, &bytesRead);
			if(BYTES_TO_READ > bytesRead) eofRecieved = true;
			counterTab[activeTab] = 0;
			cnt = 0;
		}

		while(cnt < bytesRead && dataSDin[activeTab][cnt] != '\n') ++cnt;

		if(dataSDin[activeTab][cnt] == '\n'){
			uint8_t cmdData[BYTES_TO_READ];
			uint8_t cmdLen = 0;
			if(counterTab[unactiveTab] < 50){
				cmdLen = BYTES_TO_READ - counterTab[unactiveTab];
				memcpy(cmdData, dataSDin[unactiveTab] + counterTab[unactiveTab], cmdLen);
				counterTab[unactiveTab] += cmdLen;
			}
			memcpy(cmdData + cmdLen, dataSDin[activeTab] + counterTab[activeTab], cnt - 1);
			cmdLen += cnt - counterTab[activeTab];
			counterTab[activeTab] = cnt + 1;
			cmdData[cmdLen - 1] = '\0'; // cmdData[cmdLen - 2] = '\0'; powinno usunac  ostatni znak w komendzie ale pewny nie jestem

			GCodeCommand cmd;
			parseGCodeCommand((char*)cmdData, &cmd);
			List_Push_C(BuffIN_SDcmd, &cmd, sizeof(GCodeCommand));

			++cnt;
		}
	}
}


GCodeCommand executingCmd;
void execute_command_SDcard(){
	if(executing_SDprogram && List_GetSize(BuffIN_SDcmd) > 0 && !executing_SDcommand){
		memcpy(&executingCmd, List_Front(BuffIN_SDcmd), sizeof(GCodeCommand));
		List_Pop_C(BuffIN_SDcmd);
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
		size = sprintf(data, "$MovErr: % 10.5f, % 10.5f, % 10.5f, % 10.5f\r\n",
				printerSettings.errMotor1.roundingMoveError, printerSettings.errMotor2.roundingMoveError,
				printerSettings.errMotor3.roundingMoveError, printerSettings.errMotor4.roundingMoveError);
		List_Push_C(BuffOUT_logs, data, size);
		size = sprintf(data, "$PosBef: %10.5f, %10.5f, %10.5f, %10.5f\r\n",
				motor1.data.position, motor2.data.position, motor3.data.position, motor4.data.position);
		List_Push_C(BuffOUT_logs, data, size);
#endif
		executeGCodeCommand(&executingCmd);
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


void reset_commands_SDcard(){
	if(end_SDprogram && executing_SDprogram && !executing_SDcommand && List_GetSize(BuffIN_SDcmd) == 0){
		end_SDprogram = false;
		executing_SDprogram = false;
		f_close(&file);
	}
}



/********** Handling Manager **********/

void init_manager(){
	init_operations_BT();

	//Initialize SD Card
	HAL_GPIO_WritePin(SDSPI_CS_GPIO_Port, SDSPI_CS_Pin, GPIO_PIN_SET);

	init_operations_SDcard();
}



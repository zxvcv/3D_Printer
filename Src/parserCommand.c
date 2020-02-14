/*
 * parserCommand.c
 *
 *  Created on: 13.02.2020
 *      Author: ppisk
 */
#include "parserCommand.h"
#include <string.h>
#include <stdlib.h>

#include "main.h"
extern UART_HandleTypeDef huart2; //test

void systemCmd_Motor1(SystemCommand* cmd){
	char *wsk = "systemCmd_Motor1";
	HAL_UART_Transmit(&huart2, (uint8_t*)wsk, strlen(wsk), 1000);
}

void systemCmd_Motor2(SystemCommand* cmd){
	char *wsk = "systemCmd_Motor2";
	HAL_UART_Transmit(&huart2, (uint8_t*)wsk, strlen(wsk), 1000);
}

void systemCmd_Motor3(SystemCommand* cmd){
	char *wsk = "systemCmd_Motor3";
	HAL_UART_Transmit(&huart2, (uint8_t*)wsk, strlen(wsk), 1000);
}

const struct {
	char* name;
	void (*execute)(SystemCommand*);
} systemCommands[SYSTEM_COMMANDS_NUM] = {
		{	"M1",	systemCmd_Motor1		},
		{	"M2",	systemCmd_Motor2		},
		{	"M3",	systemCmd_Motor3		}
};


void parseSystemCommand(char* cmd, SystemCommand* cmdOUT) {
	char *token = NULL, *cmdName = NULL;
	double val;
	token = strtok(cmd, " ");
	cmdName = token;
	token = strtok(NULL, " ");
	memset(cmdOUT, 0, sizeof(SystemCommand));
	while (token != NULL) {
		val = atoi(token + 1);
		switch (token[0])
		{
		//case 'X': cmdOUT->_x = val; break;
		//case 'Y': cmdOUT->_y = val; break;
		//case 'Z': cmdOUT->_z = val; break;
		//case 'E': cmdOUT->_e = val; break;
		//case 'F': cmdOUT->_f = val; break;
		//case 'S': cmdOUT->_s = val; break;
		default: break;
		}
		token = strtok(NULL, " ");
	}

	for (int i = 0; i < SYSTEM_COMMANDS_NUM; ++i) {
		if (strcmp(cmdName, systemCommands[i].name) == 0) {
			cmdOUT->execute = systemCommands[i].execute;
			break;
		}
	}
}

bool executeSystemCommand(SystemCommand* cmd) {
	if (cmd->execute == NULL)
		return false;

	cmd->execute(cmd);
	return true;
}


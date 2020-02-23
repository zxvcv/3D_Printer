/*
 * parserCommand.c
 *
 *  Created on: 13.02.2020
 *      Author: ppisk
 */
#include "parserCommand.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "a4988_stepstick.h"
#include "FIFO_void.h"

#include "main.h"
extern UART_HandleTypeDef huart2; //test
extern List* Buff_Bt_OUT;

char buffMsg[100];
uint8_t msgSize;


void systemCmd_Motor1DataRequest(SystemCommand* cmd){
	msgSize = sprintf(buffMsg, "M1D %f %f %f %f %f\n", motor1.data.position, motor1.data.positionZero, motor1.data.positionEnd, motor1.data.speed, motor1.data.maxSpeed);
	List_Push_C(Buff_Bt_OUT, (char*)buffMsg, msgSize);
}

void systemCmd_Motor1PositionMove(SystemCommand* cmd){
	double move = motorGetData(&motor1)->position - cmd->arg[0];
	motorSetMove(&motor1, move);
	motorStart(&motor1);

	while(motorIsOn(&motor1));
	systemCmd_Motor1DataRequest(cmd);

	//test
	msgSize = sprintf(buffMsg, "DONE_PosMove\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)buffMsg, msgSize, 1000);
}

void systemCmd_Motor1PositionZero(SystemCommand* cmd){
	motorGetData(&motor1)->positionZero = cmd->arg[0];
	systemCmd_Motor1DataRequest(cmd);
}

void systemCmd_Motor1PositionEnd(SystemCommand* cmd){
	motorGetData(&motor1)->positionEnd = cmd->arg[0];
	systemCmd_Motor1DataRequest(cmd);
}

void systemCmd_Motor1DistanceMove(SystemCommand* cmd){
	motorSetMove(&motor1, cmd->arg[0]);
	motorStart(&motor1);

	while(motorIsOn(&motor1));
	systemCmd_Motor1DataRequest(cmd);

	//test
	msgSize = sprintf(buffMsg, "DONE_DistMove\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)buffMsg, msgSize, 1000);
}

void systemCmd_Motor1SpeedSet(SystemCommand* cmd){
	motorGetData(&motor1)->speed = cmd->arg[0];
	systemCmd_Motor1DataRequest(cmd);

	//test
	msgSize = sprintf(buffMsg, "NewSpeed: %f %f\n", motorGetData(&motor1)->speed, cmd->arg[0]);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffMsg, msgSize, 1000);
}

void systemCmd_Motor1SpeedMax(SystemCommand* cmd){
	motorGetData(&motor1)->maxSpeed = cmd->arg[0];
	systemCmd_Motor1DataRequest(cmd);
}



void systemCmd_Motor2PositionMove(SystemCommand* cmd){
	msgSize = sprintf(buffMsg, "buff_BT_OUT_size1: %d\n", List_GetSize(Buff_Bt_OUT));
	HAL_UART_Transmit(&huart2, (uint8_t*)buffMsg, msgSize, 1000);

	msgSize = sprintf(buffMsg, "systemCmd_Motor2PositionMove\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)buffMsg, msgSize, 1000);

	msgSize = sprintf(buffMsg, "systemCmd_Motor2PositionMove");
	List_Push_C(Buff_Bt_OUT, (char*)buffMsg, msgSize);

	msgSize = sprintf(buffMsg, "buff_BT_OUT_size2: %d\n", List_GetSize(Buff_Bt_OUT));
	HAL_UART_Transmit(&huart2, (uint8_t*)buffMsg, msgSize, 1000);
}

void systemCmd_Motor2PositionZero(SystemCommand* cmd){
	msgSize = sprintf(buffMsg, "systemCmd_Motor2PositionZero");
	List_Push_C(Buff_Bt_OUT, (char*)buffMsg, msgSize);
}

void systemCmd_Motor2PositionEnd(SystemCommand* cmd){
	msgSize = sprintf(buffMsg, "systemCmd_Motor2PositionEnd");
	List_Push_C(Buff_Bt_OUT, (char*)buffMsg, msgSize);
}

void systemCmd_Motor2DistanceMove(SystemCommand* cmd){
	msgSize = sprintf(buffMsg, "systemCmd_Motor2DistanceMove");
	List_Push_C(Buff_Bt_OUT, (char*)buffMsg, msgSize);
}

void systemCmd_Motor2SpeedSet(SystemCommand* cmd){
	msgSize = sprintf(buffMsg, "systemCmd_Motor2SpeedSet");
	List_Push_C(Buff_Bt_OUT, (char*)buffMsg, msgSize);
}

void systemCmd_Motor2SpeedMax(SystemCommand* cmd){
	msgSize = sprintf(buffMsg, "systemCmd_Motor2SpeedMax");
	List_Push_C(Buff_Bt_OUT, (char*)buffMsg, msgSize);
}


void systemCmd_Motor3PositionMove(SystemCommand* cmd){

}

void systemCmd_Motor3PositionZero(SystemCommand* cmd){

}

void systemCmd_Motor3PositionEnd(SystemCommand* cmd){

}

void systemCmd_Motor3DistanceMove(SystemCommand* cmd){

}

void systemCmd_Motor3SpeedSet(SystemCommand* cmd){

}

void systemCmd_Motor3SpeedMax(SystemCommand* cmd){

}






const struct {
	char* name;
	void (*execute)(SystemCommand*);
} systemCommands[SYSTEM_COMMANDS_NUM] = {
		{	"M1PM",	systemCmd_Motor1PositionMove		},
		{	"M1PZ",	systemCmd_Motor1PositionZero		},
		{	"M1PE",	systemCmd_Motor1PositionEnd			},
		{	"M1DM",	systemCmd_Motor1DistanceMove		},
		{	"M1SS",	systemCmd_Motor1SpeedSet			},
		{	"M1SM",	systemCmd_Motor1SpeedMax			},
		{	"M1DR",	systemCmd_Motor1DataRequest			},
		{	"M2PM",	systemCmd_Motor2PositionMove		},
		{	"M2PZ",	systemCmd_Motor2PositionZero		},
		{	"M2PE",	systemCmd_Motor2PositionEnd			},
		{	"M2DM",	systemCmd_Motor2DistanceMove		},
		{	"M2SS",	systemCmd_Motor2SpeedSet			},
		{	"M2SM",	systemCmd_Motor2SpeedMax			},
		{	"M3PM",	systemCmd_Motor3PositionMove		},
		{	"M3PZ",	systemCmd_Motor3PositionZero		},
		{	"M3PE",	systemCmd_Motor3PositionEnd			},
		{	"M3DM",	systemCmd_Motor3DistanceMove		},
		{	"M3SS",	systemCmd_Motor3SpeedSet			},
		{	"M3SM",	systemCmd_Motor3SpeedMax			}

};


void parseSystemCommand(char* cmd, SystemCommand* cmdOUT) {
	char *token = NULL, *cmdName = NULL, *num;
	double val;
	int  argNum = 0;
	token = strtok(cmd, " ");
	cmdName = token;
	token = strtok(NULL, " ");
	memset(cmdOUT, 0, sizeof(SystemCommand));
	while (token != NULL || argNum >= SYSTEM_COMMANDS_ARGS_MAX_NUM) {
		num = token;
		cmdOUT->arg[argNum++] = strtod (num, NULL);

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


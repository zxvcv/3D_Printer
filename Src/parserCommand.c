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


void systemCmd_MotorDataRequest(SystemCommand* cmd){
	int motorNum;
	if(cmd->motor == &motor1)
		motorNum = 1;
	else if(cmd->motor == &motor2)
		motorNum = 2;
	else
		motorNum = 0;

	msgSize = sprintf(buffMsg, "DT M%d %f %f %f %f %f\n", motorNum, cmd->motor->data.position, cmd->motor->data.positionZero,
			cmd->motor->data.positionEnd, cmd->motor->data.speed, cmd->motor->data.maxSpeed);
	List_Push_C(Buff_Bt_OUT, (char*)buffMsg, msgSize);
}

void systemCmd_MotorPositionMove(SystemCommand* cmd){
	double move = cmd->arg[0] - cmd->motor->data.position;
	motorSetMove(cmd->motor, move);
	motorStart(cmd->motor);

	while(motorIsOn(cmd->motor));
	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorPositionZero(SystemCommand* cmd){
	cmd->motor->data.positionZero = cmd->arg[0];
	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorPositionEnd(SystemCommand* cmd){
	cmd->motor->data.positionEnd = cmd->arg[0];
	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorDistanceMove(SystemCommand* cmd){
	motorSetMove(cmd->motor, cmd->arg[0]);
	motorStart(cmd->motor);

	while(motorIsOn(cmd->motor));
	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorSpeedSet(SystemCommand* cmd){
	if(cmd->arg[0] <= cmd->motor->data.maxSpeed &&
	   cmd->arg[0] >= 0){
		cmd->motor->data.speed = cmd->arg[0];
	}

	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorSpeedMax(SystemCommand* cmd){
	if(cmd->arg[0] >= 0)
		cmd->motor->data.maxSpeed = cmd->arg[0];

	systemCmd_MotorDataRequest(cmd);
}






const struct {
	char* name;
	void (*execute)(SystemCommand*);
} systemCommands[SYSTEM_COMMANDS_NUM] = {
		{	"DR",	systemCmd_MotorDataRequest		},
		{	"PM",	systemCmd_MotorPositionMove		},
		{	"PZ",	systemCmd_MotorPositionZero		},
		{	"PE",	systemCmd_MotorPositionEnd		},
		{	"DM",	systemCmd_MotorDistanceMove		},
		{	"SS",	systemCmd_MotorSpeedSet			},
		{	"SM",	systemCmd_MotorSpeedMax			}
};


void parseSystemCommand(char* cmd, SystemCommand* cmdOUT) {
	char *token = NULL, *cmdName = NULL, *motorNum = NULL, *num = NULL;
	double val;
	int  argNum = 0;

	memset(cmdOUT, 0, sizeof(SystemCommand));

	//command name
	cmdName = strtok(cmd, " ");

	for (int i = 0; i < SYSTEM_COMMANDS_NUM; ++i) {
		if (strcmp(cmdName, systemCommands[i].name) == 0) {
			cmdOUT->execute = systemCommands[i].execute;
			break;
		}
	}

	//command subtype
	motorNum = strtok(NULL, " ");

	if(motorNum[0] == 'M'){
		//motor number
		switch(motorNum[1]){
		case '1': cmdOUT->motor = &motor1; break;
		case '2': cmdOUT->motor = &motor2; break;
		default: cmdOUT->motor = NULL; break;
		}

		//motor command arguments
		token = strtok(NULL, " ");
		while (token != NULL || argNum >= SYSTEM_COMMANDS_ARGS_MAX_NUM) {
			num = token;
			cmdOUT->arg[argNum++] = strtod (num, NULL);

			token = strtok(NULL, " ");
		}
	}
}

bool executeSystemCommand(SystemCommand* cmd) {
	if (cmd->execute == NULL)
		return false;

	cmd->execute(cmd);
	return true;
}


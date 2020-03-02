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
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i){
		msgSize = sprintf(buffMsg, "DT M%d %f %f %f %f %f\n", cmd->motor[i]->data.motorNum, cmd->motor[i]->data.position, cmd->motor[i]->data.positionZero,
				cmd->motor[i]->data.positionEnd, cmd->motor[i]->data.speed, cmd->motor[i]->data.maxSpeed);
		List_Push_C(Buff_Bt_OUT, (char*)buffMsg, msgSize);
	}
}

void systemCmd_MotorPositionMove(SystemCommand* cmd){
	double move = cmd->arg[0] - cmd->motor[0]->data.position;
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		motorSetMove(cmd->motor[i], move);
	__disable_irq();
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		motorStart(cmd->motor[i]);
	__enable_irq();

	bool state = true;
	do{
		for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
			state &= motorIsOn(cmd->motor[i]);
	}while(state);
	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorPositionValueSet(SystemCommand* cmd){
	if(cmd->arg[0] >= cmd->motor[0]->data.positionZero &&
	   cmd->arg[0] <= cmd->motor[0]->data.positionEnd ){
		cmd->motor[0]->data.position = cmd->arg[0];
	}
	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorPositionZero(SystemCommand* cmd){
	cmd->motor[0]->data.positionZero = cmd->arg[0];
	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorPositionEnd(SystemCommand* cmd){
	cmd->motor[0]->data.positionEnd = cmd->arg[0];
	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorDistanceMove(SystemCommand* cmd){
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		motorSetMove(cmd->motor[i], cmd->arg[0]);
	__disable_irq();
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		motorStart(cmd->motor[i]);
	__enable_irq();

	bool state;
	do{
		state = false;
		for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
			state |= motorIsOn(cmd->motor[i]);
	}while(state);
	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorSpeedSet(SystemCommand* cmd){
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		if(cmd->arg[0] <= cmd->motor[i]->data.maxSpeed && cmd->arg[0] >= 0)
			cmd->motor[i]->data.speed = cmd->arg[0];

	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorSpeedMax(SystemCommand* cmd){
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		if(cmd->arg[0] >= 0)
			cmd->motor[i]->data.maxSpeed = cmd->arg[0];

	systemCmd_MotorDataRequest(cmd);
}

void systemCmd_MotorsStepSizeRequest(SystemCommand* cmd){
	msgSize = sprintf(buffMsg, "SP %f %f %f %f %f\n", motor1.stepSize, motor2.stepSize, motor3.stepSize, motor4.stepSize, 0.0);
	List_Push_C(Buff_Bt_OUT, (char*)buffMsg, msgSize);
}

void systemCmd_MotorsStepSizeSet(SystemCommand* cmd){
	cmd->motor[0]->stepSize = cmd->arg[0];

	systemCmd_MotorsStepSizeRequest(cmd);
}





const struct {
	char* name;
	void (*execute)(SystemCommand*);
} systemCommands[SYSTEM_COMMANDS_NUM] = {
		{	"DR",	systemCmd_MotorDataRequest		},
		{	"PM",	systemCmd_MotorPositionMove		},
		{	"PV",	systemCmd_MotorPositionValueSet	},
		{	"PZ",	systemCmd_MotorPositionZero		},
		{	"PE",	systemCmd_MotorPositionEnd		},
		{	"DM",	systemCmd_MotorDistanceMove		},
		{	"SS",	systemCmd_MotorSpeedSet			},
		{	"SM",	systemCmd_MotorSpeedMax			},
		{	"SR",	systemCmd_MotorsStepSizeRequest	},
		{	"SP",	systemCmd_MotorsStepSizeSet		}
};


void parseSystemCommand(char* cmd, SystemCommand* cmdOUT) {
	char *token = NULL, *cmdName = NULL, *motorNum = NULL, *num = NULL;
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

	if(motorNum != NULL && motorNum[0] == 'M'){
		//motor(s) number
		uint8_t val = 0;
		for(int i=0; motorNum[i + 1] != '\0' && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i, ++val){
			switch(motorNum[i + 1]){
			case '1': cmdOUT->motor[i] = &motor1; break;
			case '2': cmdOUT->motor[i] = &motor2; break;
			case '3': cmdOUT->motor[i] = &motor3; break;
			case '4': cmdOUT->motor[i] = &motor4; break;
			default: cmdOUT->motor[i] = NULL; break;
			}
		}
		cmdOUT->motorsNum = val;

		//motor command arguments
		token = strtok(NULL, " ");
		while (token != NULL && argNum < SYSTEM_COMMANDS_ARGS_MAX_NUM) {
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


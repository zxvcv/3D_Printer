/* #######################################################################################################
 *											INTRODUCTION
 * ####################################################################################################### */
/* *********************************************************************************************************
 *  parserCommand.c
 *
 *  Created on: 13.02.2020
 *      Author: ppisk
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

#include "parserCommand.h"
#include <stdio.h>
#include <string.h>
#include "diskio.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

#define ACCURACY 1000

#define _OFFSET_MAXSPEED 0
#define _OFFSET_STEPSIZE _OFFSET_MAXSPEED+sizeof(double)
#define _OFFSET_POSITIONZERO _OFFSET_STEPSIZE+sizeof(int)
#define _OFFSET_POSITIONEND _OFFSET_POSITIONZERO+sizeof(int)



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										   PRIVATE DATA
 * ####################################################################################################### */

char buffMsg[100];
uint8_t msgSize;



/* #######################################################################################################
 *										PRIVATE DEFINITIONS
 * ####################################################################################################### */

void systemCmd_MotorDataRequest(SystemCommand* cmd, DeviceSettings* settings){
	Fifo_Err errors;

	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i){
		msgSize = sprintf(buffMsg, "DT M%d %f %f %f %f %f\n", cmd->motor[i]->data.motorNum, (double)cmd->motor[i]->data.position / ACCURACY, (double)cmd->motor[i]->data.positionZero / ACCURACY,
				(double)cmd->motor[i]->data.positionEnd / ACCURACY, cmd->motor[i]->data.speed, cmd->motor[i]->data.maxSpeed);
		list_push_C(settings->bt->Buff_Bt_OUT, (char*)buffMsg, msgSize, &errors);
	}
}

void systemCmd_MotorPositionMove(SystemCommand* cmd, DeviceSettings* settings){
	double move = cmd->arg[0] - ((double)cmd->motor[0]->data.position / ACCURACY);
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		settings->motors[i]->data.err = motorSetMove(cmd->motor[i], move);

	__disable_irq();
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		motorStart(cmd->motor[i]);
	__enable_irq();

	bool state = true;
	do{
		for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
			state &= motorIsOn(cmd->motor[i]);
	}while(state);
	systemCmd_MotorDataRequest(cmd, settings);
}

void systemCmd_MotorPositionValueSet(SystemCommand* cmd, DeviceSettings* settings){
	int argInt = (int)(cmd->arg[0] * ACCURACY);
	if(argInt >= cmd->motor[0]->data.positionZero &&
	   argInt <= cmd->motor[0]->data.positionEnd ){
		cmd->motor[0]->data.position = argInt;
	}
	systemCmd_MotorDataRequest(cmd, settings);
}

void systemCmd_MotorPositionZero(SystemCommand* cmd, DeviceSettings* settings){
	int argInt = (int)(cmd->arg[0] * ACCURACY);
	cmd->motor[0]->data.positionZero = argInt;
	EEPROM_writeData(settings->eeprom, cmd->motor[0]->eepromDataAddress + _OFFSET_POSITIONZERO, (uint8_t*)(&argInt), sizeof(argInt));
	systemCmd_MotorDataRequest(cmd, settings);
}

void systemCmd_MotorPositionEnd(SystemCommand* cmd, DeviceSettings* settings){
	int argInt = (int)(cmd->arg[0] * ACCURACY);
	cmd->motor[0]->data.positionEnd = argInt;
	EEPROM_writeData(settings->eeprom, cmd->motor[0]->eepromDataAddress + _OFFSET_POSITIONEND, (uint8_t*)(&argInt), sizeof(argInt));
	systemCmd_MotorDataRequest(cmd, settings);
}

void systemCmd_MotorDistanceMove(SystemCommand* cmd, DeviceSettings* settings){
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		settings->motors[i]->data.err = motorSetMove(cmd->motor[i], cmd->arg[0] + (double)settings->motors[i]->data.err.roundingMoveError / ACCURACY);
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
	systemCmd_MotorDataRequest(cmd, settings);
}

void systemCmd_MotorSpeedSet(SystemCommand* cmd, DeviceSettings* settings){
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		if(cmd->arg[0] <= cmd->motor[i]->data.maxSpeed && cmd->arg[0] >= 0)
			cmd->motor[i]->data.speed = cmd->arg[0];

	systemCmd_MotorDataRequest(cmd, settings);
}

void systemCmd_MotorSpeedMax(SystemCommand* cmd, DeviceSettings* settings){
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i){
		if(cmd->arg[0] >= 0)
			cmd->motor[i]->data.maxSpeed = cmd->arg[0];
		EEPROM_writeData(settings->eeprom, cmd->motor[i]->eepromDataAddress + _OFFSET_MAXSPEED, (uint8_t*)(cmd->arg), sizeof(double));
	}
	systemCmd_MotorDataRequest(cmd, settings);
}

void systemCmd_MotorsStepSizeRequest(SystemCommand* cmd, DeviceSettings* settings){
	Fifo_Err errors;

	msgSize = sprintf(buffMsg, "SP %f %f %f %f %f\n", (double)(settings->motors[0]->stepSize) / ACCURACY, (double)(settings->motors[1]->stepSize) / ACCURACY,
			(double)(settings->motors[2]->stepSize) / ACCURACY, (double)(settings->motors[3]->stepSize) / ACCURACY, (double)0);
	list_push_C(settings->bt->Buff_Bt_OUT, (char*)buffMsg, msgSize, &errors);
}

void systemCmd_MotorsStepSizeSet(SystemCommand* cmd, DeviceSettings* settings){
	int argInt = (int)(cmd->arg[0] * ACCURACY);
	cmd->motor[0]->stepSize = argInt;
	EEPROM_writeData(settings->eeprom, cmd->motor[0]->eepromDataAddress + _OFFSET_STEPSIZE, (uint8_t*)(&argInt), sizeof(argInt));
	systemCmd_MotorsStepSizeRequest(cmd, settings);
}

void systemCmd_SDCardProgramRun(SystemCommand* cmd, DeviceSettings* settings){
	f_mount(settings->fatfs, "", 0);
	extern FIL file;
	f_open(&file, "fl.txt", FA_READ);
#ifdef LOG_ENABLE
#include "manager.h"
	extern FIL logFile;
	f_open(&logFile, "logs.txt", FA_CREATE_ALWAYS | FA_WRITE);
	UINT writeSize;
	f_write(&logFile, "[START]\r\n", 9, &writeSize);
	f_sync(&logFile);
#endif

	settings->sd->executing_SDprogram = true;
}


const struct {
	char* name;
	void (*execute)(SystemCommand*, DeviceSettings*);
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
		{	"SP",	systemCmd_MotorsStepSizeSet		},
		{	"CR",	systemCmd_SDCardProgramRun		}
};



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

void parseSystemCommand(char* cmd, SystemCommand* cmdOUT, DeviceSettings* settings) {
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
			case '1': cmdOUT->motor[i] = settings->motors[0]; break;
			case '2': cmdOUT->motor[i] = settings->motors[1]; break;
			case '3': cmdOUT->motor[i] = settings->motors[2]; break;
			case '4': cmdOUT->motor[i] = settings->motors[3]; break;
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

bool executeSystemCommand(SystemCommand* cmd, DeviceSettings* settings) {
	if (cmd->execute == NULL)
		return false;

	cmd->execute(cmd, settings);
	return true;
}


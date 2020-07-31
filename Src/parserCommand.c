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

#ifdef USE_INTERRUPTS
#define IRQ_ENABLE __enable_irq()
#define IRQ_DISABLE __disable_irq()
#endif /* USE_INTERRUPTS */



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

Std_Err systemCmd_MotorDataRequest(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr = STD_ERROR;

	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
	{
		msgSize = sprintf(buffMsg, "DT M%d %f %f %f %f %f\n", 
				cmd->motor[i]->device.motorNum, 
				(double)cmd->motor[i]->data.position / ACCURACY, 
				(double)cmd->motor[i]->device.positionZero / ACCURACY,
				(double)cmd->motor[i]->device.positionEnd / ACCURACY, 
				cmd->motor[i]->data.speed, 
				cmd->motor[i]->device.maxSpeed);
		
		stdErr = fifo_push_C(settings->bt->Buff_Bt_OUT, (char*)buffMsg, msgSize + 1);
		if(stdErr != STD_OK)
		{
			return stdErr;
		}
	}
	
	return stdErr;
}

Std_Err systemCmd_MotorPositionMove(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;
	/*TODO: distinguishing between errors*/
	bool motorErr = false;

	double move = cmd->arg[0] - ((double)cmd->motor[0]->data.position / ACCURACY);
	
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
	{
		motorErr |= motorSetMove(cmd->motor[i], move, &(settings->motors[i]->data.err));
	}
	if(motorErr)
	{
		return STD_ERROR;
	}

#ifdef USE_INTERRUPTS
	IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
	{
		motorErr |= motorStart(cmd->motor[i]);
	}
	if(motorErr)
	{
		return STD_ERROR;
	}

#ifdef USE_INTERRUPTS
	IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

	bool state;
	do
	{
		state = false;
		for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		{
			state |= motorIsOn(cmd->motor[i]);
		}
	}while(state);

	stdErr = systemCmd_MotorDataRequest(cmd, settings);
	return stdErr;
}

Std_Err systemCmd_MotorPositionValueSet(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;
	/*TODO: distinguishing between errors*/

	int argInt = (int)(cmd->arg[0] * ACCURACY);
	if(argInt >= cmd->motor[0]->device.positionZero && argInt <= cmd->motor[0]->device.positionEnd)
	{
		cmd->motor[0]->data.position = argInt;
	}
	
	stdErr = systemCmd_MotorDataRequest(cmd, settings);
	return stdErr;
}

Std_Err systemCmd_MotorPositionZero(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;
	/*TODO: distinguishing between errors*/

	int argInt = (int)(cmd->arg[0] * ACCURACY);
	cmd->motor[0]->device.positionZero = argInt;
	stdErr = EEPROM_writeData(settings->eeprom, 
			cmd->motor[0]->device.eepromDataAddress + _OFFSET_POSITIONZERO, 
			(uint8_t*)(&argInt), 
			sizeof(argInt));
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	stdErr = systemCmd_MotorDataRequest(cmd, settings);
	return stdErr;
}

Std_Err systemCmd_MotorPositionEnd(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;
	/*TODO: distinguishing between errors*/

	int argInt = (int)(cmd->arg[0] * ACCURACY);
	cmd->motor[0]->device.positionEnd = argInt;
	stdErr = EEPROM_writeData(settings->eeprom, 
			cmd->motor[0]->device.eepromDataAddress + _OFFSET_POSITIONEND, 
			(uint8_t*)(&argInt), 
			sizeof(argInt));
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	stdErr = systemCmd_MotorDataRequest(cmd, settings);
	return stdErr;
}

Std_Err systemCmd_MotorDistanceMove(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;
	/*TODO: distinguishing between errors*/
	bool motorErr = false;
	
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
	{
		motorErr |= motorSetMove(cmd->motor[i], 
				cmd->arg[0] + (double)settings->motors[i]->data.err.moveError / ACCURACY, 
				&(settings->motors[i]->data.err));
	}
	if(motorErr)
	{
		return STD_ERROR;
	}

#ifdef USE_INTERRUPTS
	IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
	{
		motorErr |= motorStart(cmd->motor[i]);
	}
	if(motorErr)
	{
		return STD_ERROR;
	}

#ifdef USE_INTERRUPTS
	IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

	bool state;
	do
	{
		state = false;
		for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
		{
			state |= motorIsOn(cmd->motor[i]);
		}
	}while(state);

	stdErr = systemCmd_MotorDataRequest(cmd, settings);
	return stdErr;
}

Std_Err systemCmd_MotorSpeedSet(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;
	/*TODO: distinguishing between errors*/
	
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
	{
		if(cmd->arg[0] <= cmd->motor[i]->device.maxSpeed && cmd->arg[0] >= 0)
		{
			cmd->motor[i]->data.speed = cmd->arg[0];	
		}
	}

	stdErr = systemCmd_MotorDataRequest(cmd, settings);
	return stdErr;
}

Std_Err systemCmd_MotorSpeedMax(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;
	/*TODO: distinguishing between errors*/
	
	for(int i=0; i < cmd->motorsNum && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i)
	{
		if(cmd->arg[0] >= 0)
		{
			cmd->motor[i]->device.maxSpeed = cmd->arg[0];
		}
			
		stdErr = EEPROM_writeData(settings->eeprom, 
			cmd->motor[i]->device.eepromDataAddress + _OFFSET_MAXSPEED, 
			(uint8_t*)(cmd->arg), 
			sizeof(double));

		if(stdErr != STD_OK)
		{
			return stdErr;
		}
	}

	stdErr = systemCmd_MotorDataRequest(cmd, settings);
	return stdErr;
}

Std_Err systemCmd_MotorsStepSizeRequest(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;
	/*TODO: distinguishing between errors*/

	msgSize = sprintf(buffMsg, "SP %f %f %f %f %f\n", 
			(double)(settings->motors[0]->device.stepSize) / ACCURACY, 
			(double)(settings->motors[1]->device.stepSize) / ACCURACY,
			(double)(settings->motors[2]->device.stepSize) / ACCURACY, 
			(double)(settings->motors[3]->device.stepSize) / ACCURACY, 
			(double)0);

	stdErr = fifo_push_C(settings->bt->Buff_Bt_OUT, (char*)buffMsg, msgSize + 1);

	return stdErr;
}

Std_Err systemCmd_MotorsStepSizeSet(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;
	/*TODO: distinguishing between errors*/

	int argInt = (int)(cmd->arg[0] * ACCURACY);
	cmd->motor[0]->device.stepSize = argInt;
	stdErr = EEPROM_writeData(settings->eeprom, 
			cmd->motor[0]->device.eepromDataAddress + _OFFSET_STEPSIZE, 
			(uint8_t*)(&argInt), 
			sizeof(argInt));
	if(stdErr != STD_OK)
	{
		return stdErr;
	}

	stdErr = systemCmd_MotorsStepSizeRequest(cmd, settings);
	return stdErr;
}

Std_Err systemCmd_SDCardProgramRun(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr = STD_OK;
	/*TODO: distinguishing between errors*/
	/*TODO: add forwarding SDcard error*/

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
	return stdErr;
}


const struct {
	char* name;
	Std_Err (*execute)(SystemCommand*, DeviceSettings*);
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

Std_Err parseSystemCommand(char* cmd, SystemCommand* cmdOUT, DeviceSettings* settings)
{
	Std_Err stdErr = STD_ERROR;
	char *token = NULL, *cmdName = NULL, *motorNum = NULL, *num = NULL;
	int  argNum = 0;

	memset(cmdOUT, 0, sizeof(SystemCommand));

	//command name
	cmdName = strtok(cmd, " ");

	for (int i = 0; i < SYSTEM_COMMANDS_NUM; ++i)
	{
		if (strcmp(cmdName, systemCommands[i].name) == 0)
		{
			cmdOUT->execute = systemCommands[i].execute;
			stdErr = STD_OK;
			break;
		}
	}

	if(stdErr == STD_ERROR)
	{
		return stdErr;
	}

	//command subtype
	motorNum = strtok(NULL, " ");

	if(motorNum != NULL && motorNum[0] == 'M')
	{
		//motor(s) number
		uint8_t val = 0;
		for(int i=0; motorNum[i + 1] != '\0' && i < SYSTEM_COMMANDS_MOTORS_MAX_NUM; ++i, ++val)
		{
			switch(motorNum[i + 1])
			{
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
		while (token != NULL && argNum < SYSTEM_COMMANDS_ARGS_MAX_NUM)
		{
			num = token;
			cmdOUT->arg[argNum++] = strtod(num, NULL);

			token = strtok(NULL, " ");
		}
	}

	return stdErr;
}

Std_Err executeSystemCommand(SystemCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;

	if (cmd->execute == NULL)
	{
		return STD_PARAMETER_ERROR;
	}

	stdErr = cmd->execute(cmd, settings);
	return stdErr;
}


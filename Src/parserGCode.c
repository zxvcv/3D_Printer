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

#include "parserGCode.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "vectorOperations.h"
#include "manager.h"
#include "ProjectTypes.h"



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

#define ACCURACY 1000



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */



/* #######################################################################################################
 *											DATA TYPES
 * ####################################################################################################### */



/* #######################################################################################################
 *										PRIVATE DEFINITIONS
 * ####################################################################################################### */

/*
 * COMMAND G1
 * Linear Movement
 *
 *  Arguments:
 *	 Use X, Y, or Z values to tell the printer what position to move to.
 *	 These values will obey the current positioning mode (absolute or relative coordinates).
 *	 Include an E value if you want to move the extruder as well.
 *	 Finally, you can use an F value to tell the printer what speed (mm/min) to use for the movement.
 */
Std_Err command_G1(GCodeCommand* cmd, DeviceSettings* settings)
{
	/*TODO: check gCode state before start executing command*/
	Std_Err stdErr = STD_OK;
	bool motorErr = false;
	vect3D_d move;
	move.x = 0;
	move.y = 0;
	move.z = 0;

	if(settings->positioningMode == RELATIVE)
	{
		move.x = cmd->data.x + (double)settings->motors[0]->data.err.moveError / ACCURACY;
		move.y = cmd->data.y + (double)settings->motors[1]->data.err.moveError / ACCURACY;
		move.z = cmd->data.z + (double)settings->motors[2]->data.err.moveError / ACCURACY;
	}
	else if(settings->positioningMode == ABSOLUTE)
	{
		move.x = cmd->usedFields._x == 1 ? cmd->data.x - (double)settings->motors[0]->data.position / ACCURACY
				// + (double)motors[0].data.err.roundingMoveError / ACCURACY
				: (double)settings->motors[0]->data.err.moveError / ACCURACY;

		move.y = cmd->usedFields._y == 1 ? cmd->data.y - (double)settings->motors[1]->data.position / ACCURACY
				// + (double)motors[1].data.err.roundingMoveError / ACCURACY
				: (double)settings->motors[1]->data.err.moveError / ACCURACY;

		move.z = cmd->usedFields._z == 1 ? cmd->data.z - (double)settings->motors[2]->data.position / ACCURACY
				// + (double)motors[2].data.err.roundingMoveError / ACCURACY
				: (double)settings->motors[2]->data.err.moveError / ACCURACY;

		//clearAllMotorsRoundingErrors(&printerSettings);
	}

	if(cmd->usedFields._f == 1)
	{
		settings->speed = cmd->data.f;
	}

	vect3D_d velocity = getVelocity3D(move, settings->speed);

	settings->motors[0]->data.speed = fabs(velocity.x);
	settings->motors[1]->data.speed = fabs(velocity.y);
	settings->motors[2]->data.speed = fabs(velocity.z);
	settings->motors[3]->data.speed = fabs(velocity.z);

#ifdef LOG_ENABLE
#include "FIFO_void.h"
	extern List* BuffOUT_logs;
	uint8_t data[100], sizee;
	sizee = sprintf(data, "$CntVel: %10.5f, %10.5f, %10.5f, %10.5f\r\n", velocity.x, velocity.y, velocity.z, velocity.z);
	List_Push_C(BuffOUT_logs, data, sizee);
	sizee = sprintf(data, "$CntMov: %10.5f, %10.5f, %10.5f, %10.5f\r\n", move.x, move.y, move.z, move.z);
	List_Push_C(BuffOUT_logs, data, sizee);
#endif /*LOG_ENABLE*/

	motorErr |= motorSetMove(settings->motors[0], move.x, &(settings->motors[0]->data.err));
	motorErr |= motorSetMove(settings->motors[1], move.y, &(settings->motors[1]->data.err));
	motorErr |= motorSetMove(settings->motors[2], move.z, &(settings->motors[2]->data.err));
	motorErr |= motorSetMove(settings->motors[3], move.z, &(settings->motors[3]->data.err));

	if(motorErr)
	{
		/*TODO: check if field settings->errMove is needed*/
		settings->errMove = true;
		return STD_ERROR;
	}

#ifdef USE_INTERRUPTS
	IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

	motorErr |= motorStart(settings->motors[0]);
	motorErr |= motorStart(settings->motors[1]);
	motorErr |= motorStart(settings->motors[2]);
	motorErr |= motorStart(settings->motors[3]);

	if(motorErr)
	{
		settings->errMove = true;
		return STD_ERROR;
	}

#ifdef USE_INTERRUPTS
	IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

	bool state;
	do
	{
		state = motorIsOn(settings->motors[MOTOR_X]) | 
				motorIsOn(settings->motors[MOTOR_Y]) | 
				motorIsOn(settings->motors[MOTOR_Z1]) | 
				motorIsOn(settings->motors[MOTOR_Z2]);
	}while(state);

	settings->sdCommandState = BUSY;
	return stdErr;
}

/*
 * COMMAND G28
 * Return to home position (machine zero, aka machine reference point)
 *
 * Arguments:
 *  If no arguments are provided, the machine will home all 3 axes. 
 *  You can also specify which exact axes you want to home by adding an X, Y, or Z to the command.
 */
Std_Err command_G28(GCodeCommand* cmd, DeviceSettings* settings)
{
	/*TODO: check gCode state before start executing command*/
	Std_Err stdErr = STD_OK;
	bool motorErr = false;
	RoundingErrorData roundingError;
	
	//temporarty, in final version it could be maxSpeed of exry axis
	settings->motors[0]->data.speed = settings->speed;
	settings->motors[1]->data.speed = settings->speed;
	settings->motors[2]->data.speed = settings->speed;
	settings->motors[3]->data.speed = settings->speed;

	if(cmd->usedFields._x == 1)
	{
		motorErr = motorSetMove(settings->motors[MOTOR_X], 
								-((double)settings->motors[0]->data.position/ACCURACY), 
								&roundingError);
		if(motorErr)
		{
			return STD_ERROR;
		}
	}
		
	if(cmd->usedFields._y == 1)
	{
		motorErr = motorSetMove(settings->motors[MOTOR_Y], 
								-((double)settings->motors[1]->data.position/ACCURACY),
								&roundingError);
		if(motorErr)
		{
			return STD_ERROR;
		}
	}
		
	if(cmd->usedFields._z == 1)
	{
		motorErr = motorSetMove(settings->motors[MOTOR_Z1],
								 -((double)settings->motors[2]->data.position/ACCURACY),
								 &roundingError);
		if(motorErr)
		{
			return STD_ERROR;
		}
		
		motorErr = motorSetMove(settings->motors[MOTOR_Z2], 
								-((double)settings->motors[3]->data.position/ACCURACY),
								&roundingError);
		if(motorErr)
		{
			return STD_ERROR;
		}
	}

#ifdef USE_INTERRUPTS
	IRQ_DISABLE;
#endif /* USE_INTERRUPTS */

	motorErr |= motorStart(settings->motors[0]);
	motorErr |= motorStart(settings->motors[1]);
	motorErr |= motorStart(settings->motors[2]);
	motorErr |= motorStart(settings->motors[3]);

	if(motorErr)
	{
#ifdef USE_INTERRUPTS
		IRQ_ENABLE;
#endif /* USE_INTERRUPTS */
		return STD_ERROR;
	}

#ifdef USE_INTERRUPTS
	IRQ_ENABLE;
#endif /* USE_INTERRUPTS */

	settings->sdCommandState = BUSY;

	bool state;
	do{
		state = motorIsOn(settings->motors[MOTOR_X]) | 
				motorIsOn(settings->motors[MOTOR_Y]) | 
				motorIsOn(settings->motors[MOTOR_Z1]) | 
				motorIsOn(settings->motors[MOTOR_Z2]);
	}while(state);

	settings->sdCommandState = IDLE;

	return stdErr;
}

/*
 * COMMAND G90
 *  Positioning defined with reference to part zero
 *  (use absolute positioning for the XYZ axes)
 *
 *  Arguments:
 *	 None
 */
Std_Err command_G90(GCodeCommand* cmd, DeviceSettings* settings)
{
	/*TODO: check gCode state before start executing command*/
	settings->positioningMode = ABSOLUTE;
	return STD_OK;
}

/*
 * COMMAND G91
 *  Positioning defined with reference to previous position
 *  (use relative positioning for the XYZ axes)
 *
 *  Arguments:
 *	 None
 */
Std_Err command_G91(GCodeCommand* cmd, DeviceSettings* settings)
{
	/*TODO: check gCode state before start executing command*/
	settings->positioningMode = RELATIVE;
	return STD_OK;
}

/*
 * COMMAND G92
 *  This command sets the current axes position
 *
 *  Arguments:
 *	 Specify the absolute coordinate for any axis that you wish to overwrite. 
 *   You can include the X, Y, Z, and E axes. 
 *   If you do not include one of these axes in the command, the position will remain unchanged.
 */
Std_Err command_G92(GCodeCommand* cmd, DeviceSettings* settings)
{
	/*TODO: check gCode state before start executing command*/
	Std_Err stdErr= STD_OK;

	if(cmd->usedFields._x == 1)
	{
		/*TODO: improved accuracy*/
		settings->motors[MOTOR_X]->data.position = cmd->data.x * ACCURACY;
	}
	
	if(cmd->usedFields._y == 1)
	{
		/*TODO: improved accuracy*/
		settings->motors[MOTOR_Y]->data.position = cmd->data.y * ACCURACY;
	}
		
	if(cmd->usedFields._z == 1)
	{
		/*TODO: improved accuracy*/
		settings->motors[MOTOR_Z1]->data.position = cmd->data.z * ACCURACY;
		settings->motors[MOTOR_Z2]->data.position = cmd->data.z * ACCURACY;
	}

	/*TODO: write implementation for extruder*/
	/*if(cmd->usedFields._e == 1)
	{
		//set extruder positoin
	}*/
		
	return stdErr;
}

/*
 * COMMAND M104
 *  Use these command to set the temperature of your extruder.
 *	The M104 command starts heating the extruder, 
 *	 but then allows you to run other commands immediately afterwards.
 *
 *  Arguments:
 *	 The S value specifies the extruder temperature in degrees Celsius. 
 */
Std_Err command_M104(GCodeCommand* cmd, DeviceSettings* settings)
{
	/*TODO: check gCode state before start executing command*/
	return STD_OK;
}

/*
 * COMMAND M109
 *  Use these command to set the temperature of your extruder.
 *	The M109 command will wait until the desired temperature is reached before allowing 
 *	 any other commands to run.
 *
 *  Arguments:
 *	 The S value specifies the extruder temperature in degrees Celsius. 
 */
Std_Err command_M109(GCodeCommand* cmd, DeviceSettings* settings)
{
	/*TODO: check gCode state before start executing command*/
	return STD_OK;
}

/*
 * COMMAND M140
 *  Use these commands to set the temperature of your heated build platform.
 *	Sending the M140 command begins heating the bed, 
 *	 but allows you to run other commands immediately afterwards.
 *
 *  Arguments:
 *	 The S value specifies the bed temperature in degrees Celsius.
 */
Std_Err command_M140(GCodeCommand* cmd, DeviceSettings* settings)
{
	/*TODO: check gCode state before start executing command*/
	return STD_OK;
}

/*
 * COMMAND M190
 *  Use these commands to set the temperature of your heated build platform.
 *	The M190 command will wait until the bed temperature is reached before allowing 
 *	 any other commands to run.
 *
 *  Arguments:
 *	 The S value specifies the bed temperature in degrees Celsius.
 */
Std_Err command_M190(GCodeCommand* cmd, DeviceSettings* settings)
{
	/*TODO: check gCode state before start executing command*/
	return STD_OK;
}

/*
 * COMMAND M106
 *  This command allows you to set the speed of your printers part cooling fan.
 *	This is an external cooling fan that is pointed towards the part that you are printing.
 *
 *  Arguments:
 *	 The S value sets the speed of the cooling fan in a range between 0 (off) and 255 (full power).
 */
Std_Err command_M106(GCodeCommand* cmd, DeviceSettings* settings)
{
	/*TODO: check gCode state before start executing command*/
	return STD_OK;
}


const struct {
	char* name;
	Std_Err (*execute)(GCodeCommand*, DeviceSettings*);
} commands[GCODE_COMMANDS_NUM] = {
		{	"G1",	command_G1		},
		{	"G28",	command_G28		},
		{	"G90",	command_G90		},
		{	"G91",	command_G91		},
		{	"G92",	command_G92		},
		{	"M104",	command_M104	},
		{	"M109",	command_M109	},
		{	"M140",	command_M140	},
		{	"M190",	command_M190	},
		{	"M106",	command_M106	}
};



/* #######################################################################################################
 *										PUBLIC DEFINITIONS
 * ####################################################################################################### */

Std_Err parseGCodeCommand(char* cmd, GCodeCommand* cmdOUT)
{
	Std_Err stdErr = STD_ERROR;
	char* token = NULL, * cmdName = NULL;
	double val;

	cmdName = strtok(cmd, " ");
	token = strtok(NULL, " ");
	memset(cmdOUT, 0, sizeof(GCodeCommand));

	for (int i = 0; i < GCODE_COMMANDS_NUM; ++i) {
		if (strcmp(cmdName, commands[i].name) == 0) {
			cmdOUT->execute = commands[i].execute;
			stdErr = STD_OK;
			break;
		}
	}

	if(stdErr == STD_ERROR)
	{
		return stdErr;
	}

	while (token != NULL) {
		val = atoi(token + 1);
		switch (token[0])
		{
		case 'X': cmdOUT->data.x = val; cmdOUT->usedFields._x = 1; break;
		case 'Y': cmdOUT->data.y = val; cmdOUT->usedFields._y = 1; break;
		case 'Z': cmdOUT->data.z = val; cmdOUT->usedFields._z = 1; break;
		case 'E': cmdOUT->data.e = val; cmdOUT->usedFields._e = 1; break;
		case 'F': cmdOUT->data.f = val; cmdOUT->usedFields._f = 1; break;
		case 'S': cmdOUT->data.s = val; cmdOUT->usedFields._s = 1; break;
		default: break;
		}
		token = strtok(NULL, " ");
	}

	return stdErr;
}

Std_Err executeGCodeCommand(GCodeCommand* cmd, DeviceSettings* settings)
{
	Std_Err stdErr;
	
	if (cmd->execute == NULL)
	{
		return STD_PARAMETER_ERROR;
	}
	
	stdErr = cmd->execute(cmd, settings);
	return stdErr;
}

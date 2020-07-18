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



/* #######################################################################################################
 *											DEFINES
 * ####################################################################################################### */

#define ACCURACY 1000



/* #######################################################################################################
 *											EXTERNS
 * ####################################################################################################### */

extern DeviceSettings printerSettings; /* TO DO delete this extern */



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
GCode_Err command_G1(GCodeCommand* cmd, DeviceSettings* settings) {
	vect3D_d move;
	move.x = 0; move.y = 0; move.z = 0;

	if(settings->positioningMode == RELATIVE)
	{
		/*TO DO: commented to be compilable*/
		/*move.x = cmd->x + (double)settings->motors[0]->data.err.roundingMoveError / ACCURACY;
		move.y = cmd->y + (double)settings->motors[1]->data.err.roundingMoveError / ACCURACY;
		move.z = cmd->z + (double)settings->motors[2]->data.err.roundingMoveError / ACCURACY;*/
	}
	else if(settings->positioningMode == ABSOLUTE)
	{
		/*TO DO: commented to be compilable*/
		/*move.x = cmd->usedFields._x == 1 ? cmd->x - (double)settings->motors[0]->data.position / ACCURACY// + (double)motors[0].data.err.roundingMoveError / ACCURACY
				: (double)settings->motors[0]->data.err.roundingMoveError / ACCURACY;
		move.y = cmd->usedFields._y == 1 ? cmd->y - (double)settings->motors[1]->data.position / ACCURACY// + (double)motors[1].data.err.roundingMoveError / ACCURACY
				: (double)settings->motors[1]->data.err.roundingMoveError / ACCURACY;
		move.z = cmd->usedFields._z == 1 ? cmd->z - (double)settings->motors[2]->data.position / ACCURACY// + (double)motors[2].data.err.roundingMoveError / ACCURACY
				: (double)settings->motors[2]->data.err.roundingMoveError / ACCURACY;*/
		//clearAllMotorsRoundingErrors(&printerSettings);
	}

	if(cmd->usedFields._f == 1)
		settings->speed = cmd->f;

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

	bool error = false;
	error |= motorSetMove(settings->motors[0], move.x, &(settings->motors[0]->data.err));
	error |= motorSetMove(settings->motors[1], move.y, &(settings->motors[1]->data.err));
	error |= motorSetMove(settings->motors[2], move.z, &(settings->motors[2]->data.err));
	error |= motorSetMove(settings->motors[3], move.z, &(settings->motors[3]->data.err));


	//---------------------------------OLD---------------------------------



	if(error)
	{
		settings->errMove = true;
		return GCODE_ERROR;
	}

	__disable_irq();
	motorStart(settings->motors[0]);
	motorStart(settings->motors[1]);
	motorStart(settings->motors[2]);
	motorStart(settings->motors[3]);
	__enable_irq();

	settings->sdCommandState = BUSY;
	return GCODE_OK;
	//extern bool executing_SDcommand;
	//executing_SDcommand = true;
	/*
	bool state;
	do{
		state = motorIsOn(&(motors[0])) | motorIsOn(&(motors[1])) | motorIsOn(&(motors[2])) | motorIsOn(&(motors[3]));
	}while(state);
	*/
}

/*
 * COMMAND G28
 * Return to home position (machine zero, aka machine reference point)
 *
 * Arguments:
 *  If no arguments are provided, the machine will home all 3 axes. 
 *  You can also specify which exact axes you want to home by adding an X, Y, or Z to the command.
 */
GCode_Err command_G28(GCodeCommand* cmd, DeviceSettings* settings) {
	//temporarty, in final version it could be maxSpeed of exry axis
	settings->motors[0]->data.speed = settings->speed;
	settings->motors[1]->data.speed = settings->speed;
	settings->motors[2]->data.speed = settings->speed;
	settings->motors[3]->data.speed = settings->speed;

	/*TO DO: commented to be compilable*/
	/*if(cmd->usedFields._x == 1)
		motorSetMove(settings->motors[0], -((double)settings->motors[0]->data.position/ACCURACY));
	if(cmd->usedFields._y == 1)
		motorSetMove(settings->motors[1], -((double)settings->motors[1]->data.position/ACCURACY));
	if(cmd->usedFields._z == 1){
		motorSetMove(settings->motors[2], -((double)settings->motors[2]->data.position/ACCURACY));
		motorSetMove(settings->motors[3], -((double)settings->motors[3]->data.position/ACCURACY));
	}*/

	__disable_irq();
	motorStart(settings->motors[0]);
	motorStart(settings->motors[1]);
	motorStart(settings->motors[2]);
	motorStart(settings->motors[3]);
	__enable_irq();

	settings->sdCommandState = BUSY;
	return GCODE_OK;
	//extern bool executing_SDcommand;
	//executing_SDcommand = true;
	/*
	bool state;
	do{
		state = motorIsOn(&(motors[0])) | motorIsOn(&(motors[1])) | motorIsOn(&(motors[2])) | motorIsOn(&(motors[3]));
	}while(state);
	*/
}

/*
 * COMMAND G90
 *  Positioning defined with reference to part zero
 *  (use absolute positioning for the XYZ axes)
 *
 *  Arguments:
 *	 None
 */
GCode_Err command_G90(GCodeCommand* cmd, DeviceSettings* settings) {
	settings->positioningMode = ABSOLUTE;
	return GCODE_OK;
}

/*
 * COMMAND G91
 *  Positioning defined with reference to previous position
 *  (use relative positioning for the XYZ axes)
 *
 *  Arguments:
 *	 None
 */
GCode_Err command_G91(GCodeCommand* cmd, DeviceSettings* settings) {
	settings->positioningMode = RELATIVE;
	return GCODE_OK;
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
GCode_Err command_G92(GCodeCommand* cmd, DeviceSettings* settings) {
	if(cmd->usedFields._x == 1)
		settings->motors[0]->data.position = cmd->x; //poprawic (ACCURACY)
	if(cmd->usedFields._y == 1)
		settings->motors[1]->data.position = cmd->y; //poprawic (ACCURACY)
	if(cmd->usedFields._z == 1)
		settings->motors[2]->data.position = cmd->z; //poprawic (ACCURACY)
	//if(cmd->usedFields._e == 1)
		//set extruder positoin
	return GCODE_OK;
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
GCode_Err command_M104(GCodeCommand* cmd, DeviceSettings* settings) {
	return GCODE_OK;
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
GCode_Err command_M109(GCodeCommand* cmd, DeviceSettings* settings) {
	return GCODE_OK;
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
GCode_Err command_M140(GCodeCommand* cmd, DeviceSettings* settings) {
	return GCODE_OK;
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
GCode_Err command_M190(GCodeCommand* cmd, DeviceSettings* settings) {
	return GCODE_OK;
}

/*
 * COMMAND M106
 *  This command allows you to set the speed of your printer�s part cooling fan.
 *	This is an external cooling fan that is pointed towards the part that you are printing.
 *
 *  Arguments:
 *	 The S value sets the speed of the cooling fan in a range between 0 (off) and 255 (full power).
 */
GCode_Err command_M106(GCodeCommand* cmd, DeviceSettings* settings) {
	return GCODE_OK;
}


const struct {
	char* name;
	GCode_Err (*execute)(GCodeCommand*, DeviceSettings*);
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

GCode_Err parseGCodeCommand(char* cmd, GCodeCommand* cmdOUT) {
	char* token = NULL, * cmdName = NULL;
	double val;

	cmdName = strtok(cmd, " ");
	token = strtok(NULL, " ");
	memset(cmdOUT, 0, sizeof(GCodeCommand));
	while (token != NULL) {
		val = atoi(token + 1);
		switch (token[0])
		{
		case 'X': cmdOUT->x = val; cmdOUT->usedFields._x = 1; break;
		case 'Y': cmdOUT->y = val; cmdOUT->usedFields._y = 1; break;
		case 'Z': cmdOUT->z = val; cmdOUT->usedFields._z = 1; break;
		case 'E': cmdOUT->e = val; cmdOUT->usedFields._e = 1; break;
		case 'F': cmdOUT->f = val; cmdOUT->usedFields._f = 1; break;
		case 'S': cmdOUT->s = val; cmdOUT->usedFields._s = 1; break;
		default: break;
		}
		token = strtok(NULL, " ");
	}

	for (int i = 0; i < GCODE_COMMANDS_NUM; ++i) {
		if (strcmp(cmdName, commands[i].name) == 0) {
			cmdOUT->execute = commands[i].execute;
			break;
		}
	}
	return GCODE_OK;
}

GCode_Err executeGCodeCommand(GCodeCommand* cmd) {
	if (cmd->execute == NULL)
		return GCODE_ERROR;
	
	cmd->execute(cmd, &printerSettings);
	return GCODE_OK;
}

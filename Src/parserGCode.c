#include "parserGCode.h"
#include <string.h>
#include <stdlib.h>
#include "vectorOperations.h"
#include "a4988_stepstick.h"
#include "settings.h"
#include "projectDefines.h"

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
void command_G1(GCodeCommand* cmd) {
	vect3D_d move;

	if(printerSettings.posMode){
		move.x = cmd->usedFields._x == 1 ? cmd->x - (double)motors[0].data.position / ACCURACY// + (double)motors[0].data.err.roundingMoveError / ACCURACY
				: (double)motors[0].data.err.roundingMoveError / ACCURACY;
		move.y = cmd->usedFields._y == 1 ? cmd->y - (double)motors[1].data.position / ACCURACY// + (double)motors[1].data.err.roundingMoveError / ACCURACY
				: (double)motors[1].data.err.roundingMoveError / ACCURACY;
		move.z = cmd->usedFields._z == 1 ? cmd->z - (double)motors[2].data.position / ACCURACY// + (double)motors[2].data.err.roundingMoveError / ACCURACY
				: (double)motors[2].data.err.roundingMoveError / ACCURACY;
		//clearAllMotorsRoundingErrors(&printerSettings);
	}else{
		move.x = cmd->x + (double)motors[0].data.err.roundingMoveError / ACCURACY;
		move.y = cmd->y + (double)motors[1].data.err.roundingMoveError / ACCURACY;
		move.z = cmd->z + (double)motors[2].data.err.roundingMoveError / ACCURACY;
	}

	if(cmd->usedFields._f == 1)
		printerSettings.speed = cmd->f;

	vect3D_d velocity = getVelocity3D(move, printerSettings.speed);

	motors[0].data.speed = fabs(velocity.x);
	motors[1].data.speed = fabs(velocity.y);
	motors[2].data.speed = fabs(velocity.z);
	motors[3].data.speed = fabs(velocity.z);

	//test
	extern UART_HandleTypeDef huart2;
	uint8_t dt[100], siz;
	siz = sprintf(dt, "$CntMov: %10.5f, %10.5f, %10.5f, %10.5f\n", move.x, move.y, move.z, move.z);
	HAL_UART_Transmit(&huart2, dt, siz, 1000);
	siz = sprintf(dt, "$CntVel: %10.5f, %10.5f, %10.5f, %10.5f\n\n", velocity.x, velocity.y, velocity.z, velocity.z);
	HAL_UART_Transmit(&huart2, dt, siz, 1000);
	//test end

#ifdef LOG_ENABLE
#include "FIFO_void.h"
	extern List* BuffOUT_logs;
	uint8_t data[100], sizee;
	sizee = sprintf(data, "$CntVel: %10.5f, %10.5f, %10.5f, %10.5f\r\n", velocity.x, velocity.y, velocity.z, velocity.z);
	List_Push_C(BuffOUT_logs, data, sizee);
	sizee = sprintf(data, "$CntMov: %10.5f, %10.5f, %10.5f, %10.5f\r\n", move.x, move.y, move.z, move.z);
	List_Push_C(BuffOUT_logs, data, sizee);
#endif

	motors[0].data.err = motorSetMove(&(motors[0]), move.x);
	motors[1].data.err = motorSetMove(&(motors[1]), move.y);
	motors[2].data.err = motorSetMove(&(motors[2]), move.z);
	motors[3].data.err = motorSetMove(&(motors[3]), move.z);

	if(motors[0].data.err.errMove || motors[1].data.err.errMove || motors[2].data.err.errMove || motors[3].data.err.errMove){
		printerSettings.errMove = true;
		return;
	}

	__disable_irq();
	motorStart(&(motors[0]));
	motorStart(&(motors[1]));
	motorStart(&(motors[2]));
	motorStart(&(motors[3]));
	__enable_irq();

	extern bool executing_SDcommand;
	executing_SDcommand = true;
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
void command_G28(GCodeCommand* cmd) {
	//temporarty, in final version it could be maxSpeed of exry axis
	motors[0].data.speed = printerSettings.speed;
	motors[1].data.speed = printerSettings.speed;
	motors[2].data.speed = printerSettings.speed;
	motors[3].data.speed = printerSettings.speed;

	if(cmd->usedFields._x == 1)
		motorSetMove(&(motors[0]), -((double)motors[0].data.position/ACCURACY));
	if(cmd->usedFields._y == 1)
		motorSetMove(&(motors[1]), -((double)motors[1].data.position/ACCURACY));
	if(cmd->usedFields._z == 1){
		motorSetMove(&(motors[2]), -((double)motors[2].data.position/ACCURACY));
		motorSetMove(&(motors[3]), -((double)motors[3].data.position/ACCURACY));
	}

	__disable_irq();
	motorStart(&(motors[0]));
	motorStart(&(motors[1]));
	motorStart(&(motors[2]));
	motorStart(&(motors[3]));
	__enable_irq();

	extern bool executing_SDcommand;
	executing_SDcommand = true;
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
void command_G90(GCodeCommand* cmd) {
	printerSettings.posMode = true;
}

/*
 * COMMAND G91
 *  Positioning defined with reference to previous position
 *  (use relative positioning for the XYZ axes)
 *
 *  Arguments:
 *	 None
 */
void command_G91(GCodeCommand* cmd) {
	printerSettings.posMode = false;
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
void command_G92(GCodeCommand* cmd) {
	if(cmd->usedFields._x == 1)
		motors[0].data.position = cmd->x; //poprawic (ACCURACY)
	if(cmd->usedFields._y == 1)
		motors[1].data.position = cmd->y; //poprawic (ACCURACY)
	if(cmd->usedFields._z == 1)
		motors[2].data.position = cmd->z; //poprawic (ACCURACY)
	//if(cmd->usedFields._e == 1)
		//set extruder positoin
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
void command_M104(GCodeCommand* cmd) {

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
void command_M109(GCodeCommand* cmd) {

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
void command_M140(GCodeCommand* cmd) {

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
void command_M190(GCodeCommand* cmd) {

}

/*
 * COMMAND M106
 *  This command allows you to set the speed of your printer’s part cooling fan.
 *	This is an external cooling fan that is pointed towards the part that you are printing.
 *
 *  Arguments:
 *	 The S value sets the speed of the cooling fan in a range between 0 (off) and 255 (full power).
 */
void command_M106(GCodeCommand* cmd) {

}


const struct {
	char* name;
	void (*execute)(GCodeCommand*);
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


void parseGCodeCommand(char* cmd, GCodeCommand* cmdOUT) {
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
}

bool executeGCodeCommand(GCodeCommand* cmd) {
	if (cmd->execute == NULL)
		return false;
	
	cmd->execute(cmd);
	return true;
}

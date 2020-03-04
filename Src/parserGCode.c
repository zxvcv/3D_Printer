#include "parserGCode.h"
#include <string.h>
#include <stdlib.h>
#include "vectorOperations.h"
#include "a4988_stepstick.h"
#include "settings.h"

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
		move.x = cmd->x - motor1.data.position;
		move.y = cmd->y - motor2.data.position;
		move.z = cmd->z - motor3.data.position;
	}else{
		move.x = cmd->x;
		move.y = cmd->y;
		move.z = cmd->z;
	}

	if(cmd->usedFields._f == 1)
		printerSettings.speed = cmd->f;

	vect3D_d velocity = getVelocity3D(move, printerSettings.speed);

	motor1.data.speed = velocity.x;
	motor2.data.speed = velocity.y;
	motor3.data.speed = velocity.z;
	motor4.data.speed = velocity.z;

	motorSetMove(&motor1, move.x);
	motorSetMove(&motor2, move.y);
	motorSetMove(&motor3, move.z);
	motorSetMove(&motor4, move.z);

	__disable_irq();
	motorStart(&motor1);
	motorStart(&motor2);
	motorStart(&motor3);
	motorStart(&motor4);
	__enable_irq();

	bool state;
	do{
		state = motorIsOn(&motor1) | motorIsOn(&motor2) | motorIsOn(&motor3) | motorIsOn(&motor4);
	}while(state);
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
	motor1.data.speed = printerSettings.speed;
	motor2.data.speed = printerSettings.speed;
	motor3.data.speed = printerSettings.speed;
	motor4.data.speed = printerSettings.speed;

	if(cmd->usedFields._x == 1)
		motorSetMove(&motor1, -motor1.data.position);
	if(cmd->usedFields._y == 1)
			motorSetMove(&motor2, -motor2.data.position);
	if(cmd->usedFields._z == 1){
		motorSetMove(&motor3, -motor3.data.position);
		motorSetMove(&motor4, -motor4.data.position);
	}

	__disable_irq();
	motorStart(&motor1);
	motorStart(&motor2);
	motorStart(&motor3);
	motorStart(&motor4);
	__enable_irq();

	bool state;
	do{
		state = motorIsOn(&motor1) | motorIsOn(&motor2) | motorIsOn(&motor3) | motorIsOn(&motor4);
	}while(state);

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
		motor1.data.position = cmd->x;
	if(cmd->usedFields._y == 1)
		motor2.data.position = cmd->y;
	if(cmd->usedFields._z == 1)
		motor3.data.position = cmd->z;
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

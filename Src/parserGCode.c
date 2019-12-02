#include "parserGCode.h"
#include <string.h>
#include <stdlib.h>
#include "vectorOperations.h"

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
bool command_G1(GCodeCommand* cmd) {
	if(motorGetReset(&motor1) == STOP)  //sprawdzanie czy silnik nie jest w stanie RESET
		return false;
	while(motorIsOn(&motor1));	//oczekiwanie na zakonczenie poprzedzniego dzialania
	
	if (motorGetReset(&motor1) == START && !motorIsOn(&motor1)) {
		//obliczanie przesuniec
		vect3D_d move = { .x = cmd->_x, .y = cmd->_y, .z = cmd->_z };
		vect3D_d speed = getVelocity3D(move, cmd->_f);

		//ustawienie ruchu osiX
		//...
		//ustawienie ruchu osiY
		if(pos[1] <= printerSettings.maxPos[1]){
			motorSetMove(&motor1, move.y, speed.y);
			motorStart(&motor1);
		}
		//ustawienie ruchu osiY
		//...

		//ruch wszystkich osi razem
		//...
		while (motorIsOn(&motor1)); //czekaj az do koñca ruchu g³owicy
		return true;
	}else
		return false;
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
	char* next_token = NULL;
	double val;
	token = strtok_s(cmd, " ", &next_token);
	cmdName = token;
	token = strtok_s(NULL, " ", &next_token);
	memset(cmdOUT, 0, sizeof(GCodeCommand));
	while (token != NULL) {
		val = atoi(token + 1);
		switch (token[0])
		{
		case 'X': cmdOUT->_x = val; break;
		case 'Y': cmdOUT->_y = val; break;
		case 'Z': cmdOUT->_z = val; break;
		case 'E': cmdOUT->_e = val; break;
		case 'F': cmdOUT->_f = val; break;
		case 'S': cmdOUT->_s = val; break;
		default: break;
		}
		token = strtok_s(NULL, " ", &next_token);
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

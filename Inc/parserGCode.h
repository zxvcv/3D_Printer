/*
 * PRZYK£AD U¯YCIA:
 *
 *	char cmdName[50]; //must ends with '\0'
 *	GCodeCommand cmd;
 *	parseGCodeCommand(cmdName, &cmd);
 *	executeGCodeCommand(&cmd);
 *
 */

#ifndef PARSER_GCODE_H_
#define PARSER_GCODE_H_

#include <stdbool.h>

#define GCODE_COMMANDS_NUM 10

typedef struct GCodeCommand{
	void (*execute)(struct GCodeCommand*) ;	//command pointer
	int _x;				//X-axis move
	int _y;				//Y-axis move
	int _z;				//Z-axis move
	int _e;				//extruder-axis move
	int _f;				//speed of the movement
	int _s;				//temperature
} GCodeCommand;

void parseGCodeCommand(char* cmd, GCodeCommand* cpOUT);
bool executeGCodeCommand(GCodeCommand* cmd);

#endif // PARSER_GCODE_H_

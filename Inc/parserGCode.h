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
	int cmdNum;

	double x;				//X-axis move
	double y;				//Y-axis move
	double z;				//Z-axis move
	double e;				//extruder-axis move
	double f;				//speed of the movement
	double s;				//temperature

	struct UsedFields{
		unsigned short _x : 1;
		unsigned short _y : 1;
		unsigned short _z : 1;
		unsigned short _e : 1;
		unsigned short _f : 1;
		unsigned short _s : 1;
	}usedFields;
} GCodeCommand;

void parseGCodeCommand(char* cmd, GCodeCommand* cpOUT);
bool executeGCodeCommand(GCodeCommand* cmd);

#endif // PARSER_GCODE_H_

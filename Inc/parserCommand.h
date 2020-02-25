#ifndef _PARSER_COMMAND_H
#define _PARSER_COMMAND_H

#include <stdbool.h>
#include "a4988_stepstick.h"

#define SYSTEM_COMMANDS_NUM 7
#define SYSTEM_COMMANDS_ARGS_MAX_NUM 2

typedef struct SystemCommand{
	void (*execute)(struct SystemCommand*) ;	//command pointer
	MotorSettings *motor;
	double arg[SYSTEM_COMMANDS_ARGS_MAX_NUM];
} SystemCommand;

void parseSystemCommand(char* cmd, SystemCommand* cpOUT);
bool executeSystemCommand(SystemCommand* cmd);

#endif /* _PARSER_COMMAND_H */

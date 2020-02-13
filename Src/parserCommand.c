/*
 * parserCommand.c
 *
 *  Created on: 13.02.2020
 *      Author: ppisk
 */
#include "parserCommand.h"
#include <string.h>
#include <stdlib.h>


void systemCmd_1(SystemCommand* cmd){

}

const struct {
	char* name;
	void (*execute)(SystemCommand*);
} commands[SYSTEM_COMMANDS_NUM] = {
		{	"C1",	systemCmd_1		}
};


void parseSystemCommand(char* cmd, SystemCommand* cmdOUT) {
	char *token = NULL, *cmdName = NULL;
	char *next_token = NULL;
	double val;
	token = strtok_s(cmd, " ", &next_token);
	cmdName = token;
	token = strtok_s(NULL, " ", &next_token);
	memset(cmdOUT, 0, sizeof(SystemCommand));
	while (token != NULL) {
		val = atoi(token + 1);
		switch (token[0])
		{
		//case 'X': cmdOUT->_x = val; break;
		//case 'Y': cmdOUT->_y = val; break;
		//case 'Z': cmdOUT->_z = val; break;
		//case 'E': cmdOUT->_e = val; break;
		//case 'F': cmdOUT->_f = val; break;
		//case 'S': cmdOUT->_s = val; break;
		default: break;
		}
		token = strtok_s(NULL, " ", &next_token);
	}

	for (int i = 0; i < SYSTEM_COMMANDS_NUM; ++i) {
		if (strcmp(cmdName, commands[i].name) == 0) {
			cmdOUT->execute = commands[i].execute;
			break;
		}
	}
}

bool executeSystemCommand(SystemCommand* cmd) {
	if (cmd->execute == NULL)
		return false;

	cmd->execute(cmd);
	return true;
}


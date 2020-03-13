#ifndef MANAGER_H_
#define MANAGER_H_

#include "projectDefines.h"


/********** Bluetooth System Commands **********/
void execute_command_BT();
void parse_data_BT();
void send_command_BT();


/********** SD_Card Commands **********/
void parse_data_SDcard();
void execute_command_SDcard();
#ifdef LOG_ENABLE
void send_logs_SDcard();
#endif
void reset_commands_SDcard();


/********** Manager **********/
void init_manager();

#endif //MANAGER_H_

#include "Mock_Command_Parser.h"


Mock_Command_Parser* Mock_Command_Parser::mock;

extern "C"
{
    void init_SystemCommandsParser(BuffCommunication_Settings* buff_comm, Motor** motors,
        EEPROMSettings* eeprom, SDCard_Settings* sd, uint8_t** motor_data_addresses)
    {
        Mock_Command_Parser::mock->init_SystemCommandsParser(buff_comm, motors, eeprom, sd,
            motor_data_addresses);
    }

    Std_Err parse_SystemCommand(char* cmd, SystemCommand* cmdOUT)
    {
        return Mock_Command_Parser::mock->parse_SystemCommand(cmd, cmdOUT);
    }
}


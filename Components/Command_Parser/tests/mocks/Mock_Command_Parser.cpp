#include "Mock_Command_Parser.h"


Mock_Command_Parser* Mock_Command_Parser::mock;

extern "C"
{
    void init_SystemCommandsParser()
    {
        Mock_Command_Parser::mock->init_SystemCommandsParser();
    }

    Std_Err parse_SystemCommand(char* cmd, SystemCommand* cmdOUT)
    {
        return Mock_Command_Parser::mock->parse_SystemCommand(cmd, cmdOUT);
    }
}


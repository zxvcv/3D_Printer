#include "Mock_GCode_Parser.h"


Mock_GCode_Parser* Mock_GCode_Parser::mock;

extern "C"
{
    void init_GCodeParser(Motor** motors)
    {
        Mock_GCode_Parser::mock->init_GCodeParser(motors);
    }

    Std_Err parse_GCodeCommand(char* cmd, GCodeCommand* cmdOUT)
    {
        return Mock_GCode_Parser::mock->parse_GCodeCommand(cmd, cmdOUT);
    }
}


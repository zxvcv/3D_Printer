#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "GCode_Parser.h"
}

class Mock_GCode_Parser
{
public:
    Mock_GCode_Parser() { mock = this; }
    MOCK_METHOD1(init_GCodeParser, void(Motor** motors));
    MOCK_METHOD2(parse_GCodeCommand, Std_Err(char* cmd, GCodeCommand* cmdOUT));

    static Mock_GCode_Parser* mock;
};

#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "Command_Parser.h"
}

class Mock_Command_Parser {
public:
    Mock_Command_Parser() { mock = this; }
    MOCK_METHOD0(init_SystemCommandsParser, void());
    MOCK_METHOD2(parse_SystemCommand, Std_Err(char* cmd, SystemCommand* cmdOUT));

    static Mock_Command_Parser* mock;
};

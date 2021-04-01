#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "Manager_SDcard.h"
}

class Mock_Manager_SDcard
{
public:
    Mock_Manager_SDcard() { mock = this; }
    MOCK_METHOD2(init_manager_SDcard, Std_Err(SDCard_Settings* settings, Motor** motors));
    MOCK_METHOD1(parse_command_SDcard, Std_Err(SDCard_Settings* settings));
    MOCK_METHOD2(execute_command_SDcard, Std_Err(SDCard_Settings* settings, bool motors_state));

    static Mock_Manager_SDcard* mock;
};

#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "Manager_Communication.h"
}

class Mock_Manager_Communication
{
public:
    Mock_Manager_Communication() { mock = this; }
    MOCK_METHOD2(init_communication_manager, Std_Err(BuffCommunication_Settings* settings,
        UART_HandleTypeDef* huart));
    MOCK_METHOD1(parse_communication_command, Std_Err(BuffCommunication_Settings* settings));
    MOCK_METHOD2(execute_communication_command, Std_Err(BuffCommunication_Settings* settings,
        bool motors_state));
    MOCK_METHOD1(send_communication_command, Std_Err(BuffCommunication_Settings* settings));

    static Mock_Manager_Communication* mock;
};

#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "Buffered_Communication.h"
}

class Mock_Buffered_Communication {
public:
    Mock_Buffered_Communication() { mock = this; }
    MOCK_METHOD2(init_buffered_communication, Std_Err(BuffCommunication_Settings* settings,
        UART_HandleTypeDef* huart));
    MOCK_METHOD1(send_buffered_message, Std_Err(BuffCommunication_Settings* settings));
    MOCK_METHOD1(send_buffered_message_IT, Std_Err(BuffCommunication_Settings* settings));
    MOCK_METHOD1(receive_buffered_message_IT, Std_Err(BuffCommunication_Settings* settings));
    MOCK_METHOD1(deinit_buffered_communication, Std_Err(BuffCommunication_Settings* settings));

    static Mock_Buffered_Communication* mock;
};

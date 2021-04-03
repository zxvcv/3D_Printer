#include "Mock_Buffered_Communication.h"


Mock_Buffered_Communication* Mock_Buffered_Communication::mock;

extern "C"
{
    Std_Err init_buffered_communication(BuffCommunication_Settings* settings, UART_HandleTypeDef* huart)
    {
        return Mock_Buffered_Communication::mock->init_buffered_communication(settings, huart);
    }

    Std_Err send_buffered_message(BuffCommunication_Settings* settings)
    {
        return Mock_Buffered_Communication::mock->send_buffered_message(settings);
    }

    Std_Err send_buffered_message_IT(BuffCommunication_Settings* settings)
    {
        return Mock_Buffered_Communication::mock->send_buffered_message_IT(settings);
    }

    Std_Err receive_buffered_message_IT(BuffCommunication_Settings* settings)
    {
        return Mock_Buffered_Communication::mock->receive_buffered_message_IT(settings);
    }

    Std_Err add_message_to_send(BuffCommunication_Settings* settings, char* msg, uint8_t msgSize)
    {
        return Mock_Buffered_Communication::mock->add_message_to_send(settings, msg, msgSize);
    }

    Std_Err deinit_buffered_communication(BuffCommunication_Settings* settings)
    {
        return Mock_Buffered_Communication::mock->deinit_buffered_communication(settings);
    }
}


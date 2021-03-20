#include "Mock_Manager_Communication.h"


Mock_Manager_Communication* Mock_Manager_Communication::mock;

extern "C"
{
    Std_Err init_communication_manager(BuffCommunication_Settings* settings, UART_HandleTypeDef* huart)
    {
        return Mock_Manager_Communication::mock->init_communication_manager(settings, huart);
    }

    Std_Err parse_communication_command(BuffCommunication_Settings* settings)
    {
        return Mock_Manager_Communication::mock->parse_communication_command(settings);
    }

    Std_Err execute_communication_command(BuffCommunication_Settings* settings, bool motors_state)
    {
        return Mock_Manager_Communication::mock->execute_communication_command(settings, motors_state);
    }

    Std_Err send_communication_command(BuffCommunication_Settings* settings)
    {
        return Mock_Manager_Communication::mock->send_communication_command(settings);
    }
}


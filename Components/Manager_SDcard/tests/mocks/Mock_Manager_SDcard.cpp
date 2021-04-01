#include "Mock_Manager_SDcard.h"


Mock_Manager_SDcard* Mock_Manager_SDcard::mock;

extern "C"
{
    Std_Err init_manager_SDcard(SDCard_Settings* settings, Motor** motors)
    {
        return Mock_Manager_SDcard::mock->init_manager_SDcard(settings, motors);
    }

    Std_Err parse_command_SDcard(SDCard_Settings* settings)
    {
        return Mock_Manager_SDcard::mock->parse_command_SDcard(settings);
    }

    Std_Err execute_command_SDcard(SDCard_Settings* settings, bool motors_state)
    {
        return Mock_Manager_SDcard::mock->execute_command_SDcard(settings, motors_state);
    }
}

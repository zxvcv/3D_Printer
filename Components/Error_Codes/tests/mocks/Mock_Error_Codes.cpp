#include "Mock_Error_Codes.h"


Mock_Error_Codes* Mock_Error_Codes::mock;

extern "C"
{
    Err_Msg get_std_error_message(Std_Err err, const char *msg)
    {
        return Mock_Error_Codes::mock->get_std_error_message(err, msg);
    }

    Std_Err translate_error_hal_to_project(HAL_StatusTypeDef halStatus)
    {
        return Mock_Error_Codes::mock->translate_error_hal_to_project(halStatus);
    }
}
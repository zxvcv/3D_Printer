#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "Error_Codes.h"
}

class Mock_Error_Codes {
public:
    Mock_Error_Codes() { mock = this; }
    MOCK_METHOD2(get_std_error_message, Err_Msg(Std_Err err, const char *msg));
    MOCK_METHOD1(translate_error_hal_to_project, Std_Err(HAL_StatusTypeDef halStatus));


    static Mock_Error_Codes* mock;
};

#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "SD.h"
}

class Mock_SD
{
public:
    Mock_SD() { mock = this; }
    MOCK_METHOD4(f_read, FRESULT(FIL* fp, void* buff, UINT btr, UINT* br));

    static Mock_SD* mock;
};

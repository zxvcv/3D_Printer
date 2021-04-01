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
    MOCK_METHOD3(f_mount, FRESULT(FATFS* fs, const TCHAR* path, BYTE opt));
    MOCK_METHOD3(f_open, FRESULT(FIL* fp, const TCHAR* path, BYTE mode));

    static Mock_SD* mock;
};

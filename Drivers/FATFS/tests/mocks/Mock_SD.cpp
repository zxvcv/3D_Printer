#include "Mock_SD.h"


Mock_SD* Mock_SD::mock;

extern "C"
{
    FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br)
    {
        return Mock_SD::mock->f_read(fp, buff, btr, br);
    }
}


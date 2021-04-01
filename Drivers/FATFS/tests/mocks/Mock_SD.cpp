#include "Mock_SD.h"


Mock_SD* Mock_SD::mock;

extern "C"
{
    FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br)
    {
        return Mock_SD::mock->f_read(fp, buff, btr, br);
    }

    FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt)
    {
        return Mock_SD::mock->f_mount(fs, path, opt);
    }

    FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode)
    {
        return Mock_SD::mock->f_open(fp, path, mode);
    }
}


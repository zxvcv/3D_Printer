#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "IOpin.h"
}

class Mock_IOpin {
public:
    Mock_IOpin() { mock = this; }


    static Mock_IOpin* mock;
};

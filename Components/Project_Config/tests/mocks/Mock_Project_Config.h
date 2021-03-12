#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "Project_Config.h"
}

class Mock_Project_Config {
public:
    Mock_Project_Config() { mock = this; }

    static Mock_Project_Config* mock;
};

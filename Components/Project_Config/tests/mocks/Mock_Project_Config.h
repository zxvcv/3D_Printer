#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "Project_Config.h"
}

class Mock_Project_Config {
public:

    static Mock_Project_Config* mock;
};

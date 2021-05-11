#pragma once

#include <gmock/gmock.h>

extern "C"
{
    #include "Manager_BoundariesDetector.h"
}

class Mock_Manager_BoundariesDetector
{
public:
    Mock_Manager_BoundariesDetector() { mock = this; }

    static Mock_Manager_BoundariesDetector* mock;
};

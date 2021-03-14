#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_HAL_Drivers.h"

extern "C"
{
    #include "IOpin.h"
}

class IOpin_UT : public ::testing::Test
{
public:
    IOpin_UT(){}
    ~IOpin_UT(){}

    virtual void SetUp(){}

    virtual void TearDown(){}
};

/************************** TESTS **************************/



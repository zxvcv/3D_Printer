#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_HAL_Drivers.h"

extern "C"
{
    #include "Project_Config.h"
}

class Project_Config_UT : public ::testing::Test
{
public:
    Project_Config_UT(){}
    ~Project_Config_UT(){}

    virtual void SetUp(){}

    virtual void TearDown(){}
};

/************************** TESTS **************************/



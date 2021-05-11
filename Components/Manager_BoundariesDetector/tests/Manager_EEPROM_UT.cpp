#include <gtest/gtest.h>

#include "Mock_Project_Config.h"

using ::testing::_;
using ::testing::Return;

extern "C"
{
    #include "Manager_BoundariesDetector.h"
}

class Manager_BoundariesDetector_UT: public ::testing::Test
{
public:
    Manager_BoundariesDetector_UT() {}
    ~Manager_BoundariesDetector_UT() {}

    virtual void SetUp(){}

    virtual void TearDown(){}

    /* MOCKS */
};


/************************** TESTS **************************/

TEST_F(Manager_EEPROM_UT, get_motor_data_EEPROM_test) {}

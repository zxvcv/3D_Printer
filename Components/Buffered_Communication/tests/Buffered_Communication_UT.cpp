#include <gtest/gtest.h>

#include "Mock_FIFO_void.h"
#include "Mock_HAL_Drivers.h"

extern "C"
{
    #include "Buffered_Communication.h"
}

class Buffered_Communication_UT : public ::testing::Test {
public:
    Buffered_Communication_UT(){}
    ~Buffered_Communication_UT(){}

    virtual void SetUp() {}

    virtual void TearDown() {}
};

/************************** TESTS **************************/

TEST_F(Buffered_Communication_UT, test_test)
{

}

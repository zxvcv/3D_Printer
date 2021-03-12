#include <gtest/gtest.h>

#include "Mock_FIFO_void.h"
#include "Mock_HAL_Drivers.h"

using ::testing::_;

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

    static Mock_FIFO_void* mock_FIFO_void;
};

Mock_FIFO_void* Buffered_Communication_UT::mock_FIFO_void;

/************************** TESTS **************************/

TEST_F(Buffered_Communication_UT, test_test)
{
    BuffCommunication_Settings settings;
    UART_HandleTypeDef huart;

    EXPECT_CALL(*mock_FIFO_void, fifo_create(_)).Times(3);
    init_buffered_communication(&settings, &huart);
}

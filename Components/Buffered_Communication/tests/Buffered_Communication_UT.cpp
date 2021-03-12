#include <gtest/gtest.h>

#include "Mock_FIFO_void.h"
#include "Mock_HAL_Drivers.h"
#include "Mock_Error_Codes.h"

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
//using ::testing::WillRepeatedly;

extern "C"
{
    #include "Buffered_Communication.h"
}

class Buffered_Communication_UT : public ::testing::Test {
public:
    Buffered_Communication_UT(){
        mock_FIFO_void = new Mock_FIFO_void();
        //mock_FIFO_void->mock = mock_FIFO_void;
        mock_HAL_Drivers = new Mock_HAL_Drivers();
        mock_HAL_Drivers->mock = mock_HAL_Drivers;
        mock_Error_Codes = new Mock_Error_Codes();
        mock_Error_Codes->mock = mock_Error_Codes;
    }
    ~Buffered_Communication_UT(){
        delete mock_FIFO_void;
        delete mock_HAL_Drivers;
        delete mock_Error_Codes;
    }

    virtual void SetUp() {}

    virtual void TearDown() {}

    static Mock_FIFO_void* mock_FIFO_void;
    static Mock_HAL_Drivers* mock_HAL_Drivers;
    static Mock_Error_Codes* mock_Error_Codes;
};

Mock_FIFO_void* Buffered_Communication_UT::mock_FIFO_void;
Mock_HAL_Drivers* Buffered_Communication_UT::mock_HAL_Drivers;
Mock_Error_Codes* Buffered_Communication_UT::mock_Error_Codes;

/************************** TESTS **************************/

TEST_F(Buffered_Communication_UT, test_test)
{
    BuffCommunication_Settings settings;
    UART_HandleTypeDef huart;

    EXPECT_CALL(*mock_FIFO_void, fifo_create(_))
        .Times(3)
        .WillRepeatedly(Return(STD_OK));
    EXPECT_CALL(*mock_HAL_Drivers, HAL_UART_Receive_IT(_, _, _))
        .Times(1)
        .WillRepeatedly(Return(HAL_OK));
    init_buffered_communication(&settings, &huart);
}

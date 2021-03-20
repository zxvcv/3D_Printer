#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_FIFO_void.h"
#include "Mock_HAL_Drivers.h"
#include "Mock_Error_Codes.h"

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;

extern "C"
{
    #include "Buffered_Communication.h"
}

class Buffered_Communication_UT : public ::testing::Test
{
public:
    Buffered_Communication_UT()
    {
        mock_FIFO_void = new Mock_FIFO_void();
        mock_HAL_Drivers = new Mock_HAL_Drivers();
        mock_Error_Codes = new Mock_Error_Codes();
    }
    ~Buffered_Communication_UT()
    {
        delete mock_FIFO_void;
        delete mock_HAL_Drivers;
        delete mock_Error_Codes;
    }

    virtual void SetUp()
    {
        Std_Err stdErr;

        settings = (BuffCommunication_Settings*)malloc(sizeof(BuffCommunication_Settings));
        huart = (UART_HandleTypeDef*)malloc(sizeof(UART_HandleTypeDef));

        EXPECT_CALL(*mock_FIFO_void, fifo_create(_))
            .Times(3)
            .WillRepeatedly(Return(STD_OK));
        EXPECT_CALL(*mock_HAL_Drivers, HAL_UART_Receive_IT(huart, &(settings->recieved), 1))
            .Times(1)
            .WillOnce(Return(HAL_OK));

        stdErr = init_buffered_communication(settings, huart);
        EXPECT_EQ(stdErr, STD_OK);
    }

    virtual void TearDown()
    {
        Std_Err stdErr;

        EXPECT_CALL(*mock_FIFO_void, fifo_delete_C(_))
            .Times(3)
            .WillRepeatedly(Return(STD_OK));
        stdErr = deinit_buffered_communication(settings);
        EXPECT_EQ(stdErr, STD_OK);

        free(settings);
        free(huart);
    }

    BuffCommunication_Settings* settings;
    UART_HandleTypeDef* huart;

    /* MOCKS */
    static Mock_FIFO_void* mock_FIFO_void;
    static Mock_HAL_Drivers* mock_HAL_Drivers;
    static Mock_Error_Codes* mock_Error_Codes;
};

Mock_FIFO_void* Buffered_Communication_UT::mock_FIFO_void;
Mock_HAL_Drivers* Buffered_Communication_UT::mock_HAL_Drivers;
Mock_Error_Codes* Buffered_Communication_UT::mock_Error_Codes;

/************************** TESTS **************************/

TEST_F(Buffered_Communication_UT, send_buffered_message_sending)
{
    Std_Err stdErr;

    settings->transmission = false; //there is no transmission ongoing

    EXPECT_CALL(*mock_FIFO_void, fifo_getSize(settings->Buff_OUT))
        .Times(1)
        .WillOnce(Return(1));
    EXPECT_CALL(*mock_FIFO_void, fifo_front(settings->Buff_OUT, _))
        .Times(1)
        .WillOnce(Return(STD_OK));
    EXPECT_CALL(*mock_FIFO_void, fifo_getDataSize(settings->Buff_OUT))
        .Times(1)
        .WillOnce(Return(0));
    EXPECT_CALL(*mock_HAL_Drivers, HAL_UART_Transmit_IT(huart, _, _))
        .Times(1)
        .WillOnce(Return(HAL_OK));
    EXPECT_CALL(*mock_Error_Codes, translate_error_hal_to_project(_))
        .Times(1)
        .WillOnce(Return(STD_OK));

    stdErr = send_buffered_message(settings);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_EQ(settings->transmission, true);
}

TEST_F(Buffered_Communication_UT, send_buffered_message_busy)
{
    Std_Err stdErr;

    settings->transmission = true; //there is transmission ongoing

    stdErr = send_buffered_message(settings);
    EXPECT_EQ(stdErr, STD_BUSY_ERROR);

    stdErr = send_buffered_message(settings);
    EXPECT_EQ(stdErr, STD_BUSY_ERROR);
}

TEST_F(Buffered_Communication_UT, send_buffered_message_queueEmpty)
{
    Std_Err stdErr;

    settings->transmission = false; //there is no transmission ongoing

    EXPECT_CALL(*mock_FIFO_void, fifo_getSize(settings->Buff_OUT))
        .Times(1)
        .WillOnce(Return(0));
    stdErr = send_buffered_message(settings);
    EXPECT_EQ(stdErr, STD_OK);
}

TEST_F(Buffered_Communication_UT, send_buffered_message_IT_nextMessage)
{
    Std_Err stdErr;

    settings->transmission = true; //there is transmission ongoing

    EXPECT_CALL(*mock_FIFO_void, fifo_pop_C(settings->Buff_OUT))
        .Times(1)
        .WillOnce(Return(STD_OK));
    EXPECT_CALL(*mock_FIFO_void, fifo_getSize(settings->Buff_OUT))
        .Times(1)
        .WillOnce(Return(1));
    EXPECT_CALL(*mock_FIFO_void, fifo_front(settings->Buff_OUT, _))
        .Times(1)
        .WillOnce(Return(STD_OK));
    EXPECT_CALL(*mock_FIFO_void, fifo_getDataSize(settings->Buff_OUT))
        .Times(1)
        .WillOnce(Return(0));
    EXPECT_CALL(*mock_HAL_Drivers, HAL_UART_Transmit_IT(huart, _, _))
        .Times(1)
        .WillOnce(Return(HAL_OK));
    EXPECT_CALL(*mock_Error_Codes, translate_error_hal_to_project(_))
        .Times(1)
        .WillOnce(Return(STD_OK));

    stdErr = send_buffered_message_IT(settings);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_EQ(settings->transmission, true);
}

TEST_F(Buffered_Communication_UT, send_buffered_message_IT_noMoreMessages)
{
    Std_Err stdErr;

    settings->transmission = true; //there is transmission ongoing

    EXPECT_CALL(*mock_FIFO_void, fifo_pop_C(settings->Buff_OUT))
        .Times(1)
        .WillOnce(Return(STD_OK));
    EXPECT_CALL(*mock_FIFO_void, fifo_getSize(settings->Buff_OUT))
        .Times(1)
        .WillOnce(Return(0));

    stdErr = send_buffered_message_IT(settings);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_EQ(settings->transmission, false);
}

TEST_F(Buffered_Communication_UT, receive_buffered_message_IT_recieveStandardChar)
{
    Std_Err stdErr;

    settings->recieved = '*';

    EXPECT_CALL(*mock_FIFO_void, fifo_push_C(settings->Buff_IN, &(settings->recieved), 1))
        .Times(1)
        .WillOnce(Return(STD_OK));
    EXPECT_CALL(*mock_HAL_Drivers, HAL_UART_Receive_IT(settings->huart, &(settings->recieved), 1))
        .Times(1)
        .WillOnce(Return(HAL_OK));
    EXPECT_CALL(*mock_Error_Codes, translate_error_hal_to_project(_))
        .Times(1)
        .WillOnce(Return(STD_OK));

    stdErr = receive_buffered_message_IT(settings);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_EQ(settings->EOL_recieved, false);
}

TEST_F(Buffered_Communication_UT, receive_buffered_message_IT_recieveNewLineChar)
{
    Std_Err stdErr;

    settings->recieved = '\n';

    EXPECT_CALL(*mock_FIFO_void, fifo_push_C(settings->Buff_IN, &(settings->recieved), 1))
        .Times(1)
        .WillOnce(Return(STD_OK));
    EXPECT_CALL(*mock_HAL_Drivers, HAL_UART_Receive_IT(settings->huart, &(settings->recieved), 1))
        .Times(1)
        .WillOnce(Return(HAL_OK));
    EXPECT_CALL(*mock_Error_Codes, translate_error_hal_to_project(_))
        .Times(1)
        .WillOnce(Return(STD_OK));

    stdErr = receive_buffered_message_IT(settings);
    EXPECT_EQ(stdErr, STD_OK);
    EXPECT_EQ(settings->EOL_recieved, true);
}

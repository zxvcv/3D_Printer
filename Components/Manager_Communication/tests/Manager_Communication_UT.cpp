#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_Command_Parser.h"
#include "Mock_Buffered_Communication.h"

using ::testing::_;
using ::testing::Return;

extern "C"
{
    #include "Manager_Communication.h"
}

class Manager_Communication_UT: public ::testing::Test
{
public:
    Manager_Communication_UT()
    {
        mock_Command_Parser = new Mock_Command_Parser();
        mock_Buffered_Communication = new Mock_Buffered_Communication();
    }
    ~Manager_Communication_UT()
    {
        delete mock_Command_Parser;
        delete mock_Buffered_Communication;
    }

    virtual void SetUp()
    {
        Std_Err stdErr;

        buff_comm = (BuffCommunication_Settings*)malloc(sizeof(BuffCommunication_Settings));
        huart = (UART_HandleTypeDef*)malloc(sizeof(UART_HandleTypeDef));

        EXPECT_CALL(*mock_Buffered_Communication, init_buffered_communication(buff_comm, huart))
            .Times(1)
            .WillOnce(Return(STD_OK));

        stdErr = init_communication_manager(buff_comm, huart);
        EXPECT_EQ(stdErr, STD_OK);
    }

    virtual void TearDown()
    {
        free(buff_comm);
        free(huart);
    }

    BuffCommunication_Settings* buff_comm;
    UART_HandleTypeDef* huart;

    /* MOCKS */
    static Mock_Command_Parser* mock_Command_Parser;
    static Mock_Buffered_Communication* mock_Buffered_Communication;
};

Mock_Command_Parser* Manager_Communication_UT::mock_Command_Parser;
Mock_Buffered_Communication* Manager_Communication_UT::mock_Buffered_Communication;

/************************** TESTS **************************/

TEST_F(Manager_Communication_UT, parse_communication_command_test)
{
    //TODO ...
}

TEST_F(Manager_Communication_UT, execute_communication_command_test)
{
    //TODO ...
}

TEST_F(Manager_Communication_UT, send_communication_command_test)
{
    Std_Err stdErr;

    EXPECT_CALL(*mock_Buffered_Communication, send_buffered_message(buff_comm))
        .Times(1)
        .WillOnce(Return(STD_OK));

    stdErr = send_communication_command(buff_comm);
    EXPECT_EQ(stdErr, STD_OK);
}

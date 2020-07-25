#include <gtest/gtest.h>
#include <gmock/gmock.h>
//#include <stdint.h>

using ::testing::_;
//using ::testing::Assign;
using ::testing::Return;
//using ::testing::SetArrayArgument;


extern "C"
{
    #include "managerBT.h"
}



class Mock_managerBT {
public:
    MOCK_METHOD3(HAL_UART_Receive_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, 
        uint16_t Size));
    MOCK_METHOD3(HAL_UART_Transmit_IT, HAL_StatusTypeDef(UART_HandleTypeDef *huart, uint8_t *pData, 
        uint16_t Size));
};

class ManagerBT_test : public ::testing::Test {
public:
    ManagerBT_test()
    {
        mock = new Mock_managerBT();
    }
    ~ManagerBT_test()
    {
        delete mock;
    }

    virtual void SetUp()
    {
        bt = (BT_Settings*)malloc(sizeof(BT_Settings));
        setupBT(bt);
    }

    virtual void TearDown()
    {
        free(bt);
    }


    void setupBT(BT_Settings* bt);


    BT_Settings* bt;

    static Mock_managerBT* mock;
};

Mock_managerBT* ManagerBT_test::mock;



/************************** TESTS **************************/

TEST_F(ManagerBT_test, ManagerBT__correctInit__test)
{
    Std_Err stdErr;

    EXPECT_CALL(*mock, HAL_UART_Receive_IT(_, &(bt->recievedBT), 1))
        .WillOnce(Return(HAL_OK));

    stdErr = init_operations_BT(bt);
    EXPECT_EQ(stdErr, STD_OK);

    //EXPECT_NE(bt->Buff_Bt_IN, NULL);
    //EXPECT_NE(bt->Buff_Bt_OUT, NULL);
    //EXPECT_NE(bt->Buff_InputCommandsBT, NULL);
}



/************************** PUBLIC FUNCTIONS **************************/

void ManagerBT_test::setupBT(BT_Settings* bt)
{
    bt->Buff_Bt_IN = NULL;
    bt->Buff_Bt_OUT = NULL;
    bt->Buff_InputCommandsBT = NULL;
    bt->EOL_BT_recieved = false;
    bt->transmissionBT = false;
}



/************************** MOCK DEFINITIONS **************************/

extern "C"
{
    HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        ManagerBT_test::mock->HAL_UART_Receive_IT(huart, pData, Size);
    }
    HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
    {
        ManagerBT_test::mock->HAL_UART_Transmit_IT(huart, pData, Size);
    }
}


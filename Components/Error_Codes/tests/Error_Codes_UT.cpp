#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_HAL_Drivers.h"

#define TEST_TAB_SIZE 3
#define ERR_NUM 9

extern "C"
{
    #include "Error_Codes.h"
}

class Error_Codes_UT : public ::testing::Test
{
public:
    Error_Codes_UT(){}
    ~Error_Codes_UT(){}

    virtual void SetUp(){}

    virtual void TearDown(){}

    char* test_tab[TEST_TAB_SIZE] = {
        (char*)"Test error message",
        (char*)"",
        NULL
    };
};

/************************** TESTS **************************/

TEST_F(Error_Codes_UT, create_error_code)
{
    Err_Msg err_msg;

    for(int i=0; i<TEST_TAB_SIZE; ++i)
    {
        for(int err=0; err<ERR_NUM; ++err)
        {
            err_msg = get_std_error_message(static_cast<Std_Err>(err), test_tab[i]);
            EXPECT_STREQ(test_tab[i], err_msg.msg);
            EXPECT_EQ(static_cast<Std_Err>(err), err_msg.err);
        }
    }
}

TEST_F(Error_Codes_UT, translate_error_hal_to_project)
{
    Std_Err proj_err;

    proj_err = translate_error_hal_to_project(HAL_OK);
    EXPECT_EQ(STD_OK, proj_err);

    proj_err = translate_error_hal_to_project(HAL_ERROR);
    EXPECT_EQ(STD_ERROR, proj_err);

    proj_err = translate_error_hal_to_project(HAL_BUSY);
    EXPECT_EQ(STD_BUSY_ERROR, proj_err);

    proj_err = translate_error_hal_to_project(HAL_TIMEOUT);
    EXPECT_EQ(STD_TIMEOUT_ERROR, proj_err);

    proj_err = translate_error_hal_to_project(static_cast<HAL_StatusTypeDef>(0xffU));
    EXPECT_EQ(STD_ERROR, proj_err);
}

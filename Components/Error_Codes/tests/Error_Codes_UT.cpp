#include <gtest/gtest.h>

#define TAB_SIZE 3
#define ERR_NUM 9

extern "C"
{
    #include "Error_Codes.h"
}

class Error_Codes_test : public ::testing::Test {
public:
    Error_Codes_test(){}
    ~Error_Codes_test(){}
    
    virtual void SetUp(){}

    virtual void TearDown(){}

    char* tab[TAB_SIZE] = {
        (char*)"Test error message",
        (char*)"",
        NULL
    };
};

/************************** TESTS **************************/

TEST_F(Error_Codes_test, create_error_code) 
{
    Err_Msg err_msg;

    for(int i=0; i<TAB_SIZE; ++i)
    {
        for(int err=0; err<ERR_NUM; ++err)
        {
            err_msg = get_std_error_message(static_cast<Std_Err>(err), tab[i]);
            EXPECT_STREQ(tab[i], err_msg.msg);
            EXPECT_EQ(static_cast<Std_Err>(err), err_msg.err);
        }
    }
}

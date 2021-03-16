#include <gtest/gtest.h>

#include "Mock_Project_Config.h"

using ::testing::_;

extern "C"
{
    #include "Command_Parser.h"
}

class Command_Parser_UT : public ::testing::Test
{
public:
    Command_Parser_UT() {}
    ~Command_Parser_UT() {}

    virtual void SetUp() {}

    virtual void TearDown() {}


    /* MOCKS */

};



/************************** TESTS **************************/

TEST_F(Command_Parser_UT, test)
{

}


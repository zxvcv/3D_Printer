#include <gtest/gtest.h>

#include "Mock_Project_Config.h"

using ::testing::_;

extern "C"
{
    #include "GCode_Parser.h"
}

class GCode_Parser_UT: public ::testing::Test
{
public:
    GCode_Parser_UT() {}
    ~GCode_Parser_UT() {}

    virtual void SetUp() {}

    virtual void TearDown() {}

    /* MOCKS */
};





/************************** TESTS **************************/

TEST_F(GCode_Parser_UT, test)
{

}

#include <gtest/gtest.h>

#include "Mock_Project_Config.h"

using ::testing::_;

extern "C"
{
    #include "Command_Parser.h"
    #include "_commands.h"
    #include "stubs_CommandPrivate.h"
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

TEST_F(Command_Parser_UT, parser_test)
{
    Std_Err stdErr;

    #define TEST_VALUES_LEN 10
    char cmdStr0[] = "1U00";
    char cmdStr1[] = "1U10";
    char cmdStr2[] = "1U11";
    char cmdStr3[] = "1U20";
    char cmdStr4[] = "1U21";
    char cmdStr5[] = "1U22";
    char cmdStr6[] = "1U23";
    char cmdStr7[] = "1U24";
    char cmdStr8[] = "1U25";
    char cmdStr9[] = "1U40";
    struct{
        char* cmd;

        Std_Err expected_exit_val;
        Std_Err (*expected_init)(SystemCommand*);
        Std_Err (*expected_remove)(SystemCommand*);
        Std_Err (*expected_step)(SystemCommand*);
        struct{
            double x;       //X-axis
            double y;       //Y-axis
            double z;       //Z-axis
            double e;       //extruder-axis
        }expected_data;
        uint8_t expected_used_fields;
    }test_values[TEST_VALUES_LEN] = {
        { cmdStr0, STD_OK, init_U00, NULL, NULL, { 0., 0., 0., 0. }, 0x00 },
        { cmdStr1, STD_OK, init_U10, NULL, step_U10, { 0., 0., 0., 0. }, 0x00 },
        { cmdStr2, STD_OK, init_U11, NULL, step_U11, { 0., 0., 0., 0. }, 0x00 },
        { cmdStr3, STD_OK, init_U20, NULL, NULL, { 0., 0., 0., 0. }, 0x00 },
        { cmdStr4, STD_OK, init_U21, NULL, NULL, { 0., 0., 0., 0. }, 0x00 },
        { cmdStr5, STD_OK, init_U22, NULL, NULL, { 0., 0., 0., 0. }, 0x00 },
        { cmdStr6, STD_OK, init_U23, NULL, NULL, { 0., 0., 0., 0. }, 0x00 },
        { cmdStr7, STD_OK, init_U24, NULL, NULL, { 0., 0., 0., 0. }, 0x00 },
        { cmdStr8, STD_OK, init_U25, NULL, NULL, { 0., 0., 0., 0. }, 0x00 },
        { cmdStr9, STD_OK, init_U40, NULL, NULL, { 0., 0., 0., 0. }, 0x00 }
    };

    SystemCommand cmdOUT;

    for(int i=0; i<TEST_VALUES_LEN; ++i)
    {

        stdErr = parse_SystemCommand(test_values[i].cmd, &cmdOUT);
        EXPECT_EQ(stdErr, test_values[i].expected_exit_val);

        EXPECT_NEAR(cmdOUT.data.x, test_values[i].expected_data.x, 1/ACCURACY);
        EXPECT_NEAR(cmdOUT.data.y, test_values[i].expected_data.y, 1/ACCURACY);
        EXPECT_NEAR(cmdOUT.data.z, test_values[i].expected_data.z, 1/ACCURACY);
        EXPECT_NEAR(cmdOUT.data.e, test_values[i].expected_data.e, 1/ACCURACY);

        EXPECT_EQ(cmdOUT.used_fields, test_values[i].expected_used_fields);
    }
}

TEST_F(Command_Parser_UT, U00_test)
{
    Std_Err stdErr;

    char cmdStr[] = "1U00";

    SystemCommand cmdOUT;

    stdErr = parse_SystemCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(Command_Parser_UT, U10_test)
{
    Std_Err stdErr;

    char cmdStr[] = "1U10";

    SystemCommand cmdOUT;

    stdErr = parse_SystemCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    stdErr = cmdOUT.step(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    // EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(Command_Parser_UT, U11_test)
{
    Std_Err stdErr;

    char cmdStr[] = "1U11";

    SystemCommand cmdOUT;

    stdErr = parse_SystemCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    stdErr = cmdOUT.step(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    // EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(Command_Parser_UT, U20_test)
{
    Std_Err stdErr;

    char cmdStr[] = "1U20";

    SystemCommand cmdOUT;

    stdErr = parse_SystemCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(Command_Parser_UT, U21_test)
{
    Std_Err stdErr;

    char cmdStr[] = "1U21";

    SystemCommand cmdOUT;

    stdErr = parse_SystemCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(Command_Parser_UT, U22_test)
{
    Std_Err stdErr;

    char cmdStr[] = "1U22";

    SystemCommand cmdOUT;

    stdErr = parse_SystemCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(Command_Parser_UT, U23_test)
{
    Std_Err stdErr;

    char cmdStr[] = "1U23";

    SystemCommand cmdOUT;

    stdErr = parse_SystemCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(Command_Parser_UT, U24_test)
{
    Std_Err stdErr;

    char cmdStr[] = "1U24";

    SystemCommand cmdOUT;

    stdErr = parse_SystemCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(Command_Parser_UT, U25_test)
{
    Std_Err stdErr;

    char cmdStr[] = "1U25";

    SystemCommand cmdOUT;

    stdErr = parse_SystemCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(Command_Parser_UT, U40_test)
{
    Std_Err stdErr;

    char cmdStr[] = "1U40";

    SystemCommand cmdOUT;

    stdErr = parse_SystemCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

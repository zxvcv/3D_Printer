#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_A4988_stepstick.h"

using ::testing::_;

extern "C"
{
    #include "GCode_Parser.h"
    #include "_commands.h"
    #include "stubs_GCodePrivate.h"
}

class GCode_Parser_UT: public ::testing::Test
{
public:
    GCode_Parser_UT()
    {
        mock_A4988_stepstick = new Mock_A4988_stepstick();
    }
    ~GCode_Parser_UT()
    {
        delete mock_A4988_stepstick;
    }

    virtual void SetUp() {}

    virtual void TearDown() {}

    /* MOCKS */
    static Mock_A4988_stepstick* mock_A4988_stepstick;
};

Mock_A4988_stepstick* GCode_Parser_UT::mock_A4988_stepstick;



/************************** TESTS **************************/

TEST_F(GCode_Parser_UT, parser_test)
{
    Std_Err stdErr;

    #define TEST_VALUES_LEN 11
    char cmdStr0[] = "G1 X10 Y15 Z20 E5 F10";
    char cmdStr1[] = "G28";
    char cmdStr2[] = "G90";
    char cmdStr3[] = "G91";
    char cmdStr4[] = "G92 X10 Y20 Z30 E15";
    char cmdStr5[] = "M104 S80";
    char cmdStr6[] = "M109 S80";
    char cmdStr7[] = "M140 S80";
    char cmdStr8[] = "M190 S80";
    char cmdStr9[] = "M106 S100";
    char cmdStr10[] = "AAA BBB CCC DDD";
    struct{
        char* cmd;

        Std_Err expected_exit_val;
        Std_Err (*expected_init)(GCodeCommand*);
        Std_Err (*expected_remove)(GCodeCommand*);
        Std_Err (*expected_step)(GCodeCommand*);
        struct{
            double x;       //X-axis move
            double y;       //Y-axis move
            double z;       //Z-axis move
            double e;       //extruder-axis move
            double f;       //speed of the movement
            double s;       //temperature
        }expected_data;
        uint8_t expected_used_fields;
    }test_values[TEST_VALUES_LEN] = {
        { cmdStr0, STD_OK, init_G1, NULL, step_G1, { 10., 15., 20., 5., 10., 0. }, 0x1f },
        { cmdStr1, STD_OK, init_G28, NULL, step_G28, { 0., 0., 0., 0., 0., 0. }, 0x00 },
        { cmdStr2, STD_OK, init_G90, NULL, NULL, { 0., 0., 0., 0., 0., 0. }, 0x00 },
        { cmdStr3, STD_OK, init_G91, NULL, NULL, { 0., 0., 0., 0., 0., 0. }, 0x00 },
        { cmdStr4, STD_OK, init_G92, NULL, NULL, { 10., 20., 30., 15., 0., 0. }, 0x0f },
        { cmdStr5, STD_OK, init_M104, NULL, NULL, { 0., 0., 0., 0., 0., 80. }, 0x20 },
        { cmdStr6, STD_OK, init_M109, NULL, NULL, { 0., 0., 0., 0., 0., 80. }, 0x20 },
        { cmdStr7, STD_OK, init_M140, NULL, NULL, { 0., 0., 0., 0., 0., 80. }, 0x20 },
        { cmdStr8, STD_OK, init_M190, NULL, NULL, { 0., 0., 0., 0., 0., 80. }, 0x20 },
        { cmdStr9, STD_OK, init_M106, NULL, NULL, { 0., 0., 0., 0., 0., 100. }, 0x20 },
        { cmdStr10, STD_ERROR, NULL, NULL, NULL, { 0., 0., 0., 0., 0., 0. }, 0x00 }
    };

    GCodeCommand cmdOUT;

    for(int i=0; i<TEST_VALUES_LEN; ++i)
    {

        stdErr = parse_GCodeCommand(test_values[i].cmd, &cmdOUT);
        EXPECT_EQ(stdErr, test_values[i].expected_exit_val);

        EXPECT_NEAR(cmdOUT.data.x, test_values[i].expected_data.x, 1/ACCURACY);
        EXPECT_NEAR(cmdOUT.data.y, test_values[i].expected_data.y, 1/ACCURACY);
        EXPECT_NEAR(cmdOUT.data.z, test_values[i].expected_data.z, 1/ACCURACY);
        EXPECT_NEAR(cmdOUT.data.e, test_values[i].expected_data.e, 1/ACCURACY);
        EXPECT_NEAR(cmdOUT.data.f, test_values[i].expected_data.f, 1/ACCURACY);
        EXPECT_NEAR(cmdOUT.data.s, test_values[i].expected_data.s, 1/ACCURACY);

        EXPECT_EQ(cmdOUT.used_fields, test_values[i].expected_used_fields);
    }
}

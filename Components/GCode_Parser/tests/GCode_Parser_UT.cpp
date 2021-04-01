#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_A4988_stepstick.h"
#include "Mock_Vector_Math.h"

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
        mock_Vector_Math = new Mock_Vector_Math();
    }
    ~GCode_Parser_UT()
    {
        delete mock_A4988_stepstick;
        delete mock_Vector_Math;
    }

    virtual void SetUp()
    {
        for(int i=0; i<MOTORS_NUM; ++i)
        {
            motors[i] = (Motor*)malloc(sizeof(Motor));
        }

        init_GCodeParser(motors);
    }

    virtual void TearDown()
    {
        for(int i=0; i<MOTORS_NUM; ++i)
        {
            free(motors[i]);
        }
    }

    Motor* motors[MOTORS_NUM];

    /* MOCKS */
    static Mock_A4988_stepstick* mock_A4988_stepstick;
    static Mock_Vector_Math* mock_Vector_Math;
};

Mock_A4988_stepstick* GCode_Parser_UT::mock_A4988_stepstick;
Mock_Vector_Math* GCode_Parser_UT::mock_Vector_Math;



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

TEST_F(GCode_Parser_UT, G1_test)
{
    Std_Err stdErr;

    char cmdStr[] = "G1 X10 Y15 Z20 E5 F10";

    GCodeCommand cmdOUT;

    stdErr = parse_GCodeCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);


    EXPECT_CALL(*mock_Vector_Math, getVelocity3D(_, _));
    EXPECT_CALL(*mock_A4988_stepstick, motor_get_linear_move_settings(_, _, _, ACCURACY, _, _))
        .Times(MOTORS_NUM);
    EXPECT_CALL(*mock_A4988_stepstick, motor_set_counters(_, _))
        .Times(MOTORS_NUM);
    EXPECT_CALL(*mock_A4988_stepstick, motor_set_direction(_, _))
        .Times(MOTORS_NUM);
    EXPECT_CALL(*mock_A4988_stepstick, motor_start(_))
        .Times(MOTORS_NUM);

    stdErr = cmdOUT.step(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(GCode_Parser_UT, G28_test)
{
    Std_Err stdErr;

    char cmdStr[] = "G28";

    GCodeCommand cmdOUT;

    stdErr = parse_GCodeCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);


    EXPECT_CALL(*mock_Vector_Math, getVelocity3D(_, _));
    EXPECT_CALL(*mock_A4988_stepstick, motor_get_linear_move_settings(_, _, _, ACCURACY, _, _))
        .Times(MOTORS_NUM);
    EXPECT_CALL(*mock_A4988_stepstick, motor_set_counters(_, _))
        .Times(MOTORS_NUM);
    EXPECT_CALL(*mock_A4988_stepstick, motor_set_direction(_, _))
        .Times(MOTORS_NUM);
    EXPECT_CALL(*mock_A4988_stepstick, motor_start(_))
        .Times(MOTORS_NUM);

    stdErr = cmdOUT.step(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(GCode_Parser_UT, G90_test)
{
    Std_Err stdErr;

    char cmdStr[] = "G90";

    GCodeCommand cmdOUT;

    stdErr = parse_GCodeCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    // EXPECT_EQ(global_gcode_settings.positioning_mode, ABSOLUTE);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(GCode_Parser_UT, G91_test)
{
    Std_Err stdErr;

    char cmdStr[] = "G91";

    GCodeCommand cmdOUT;

    stdErr = parse_GCodeCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    // EXPECT_EQ(global_gcode_settings.positioning_mode, RELATIVE);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(GCode_Parser_UT, G92_test)
{
    Std_Err stdErr;

    char cmdStr[] = "G92 X10 Y20 Z30 E15";

    GCodeCommand cmdOUT;

    stdErr = parse_GCodeCommand(cmdStr, &cmdOUT);

    EXPECT_CALL(*mock_A4988_stepstick, motor_set_position(_, _))
        .Times(MOTORS_NUM);
    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(GCode_Parser_UT, M104_test)
{
    Std_Err stdErr;

    char cmdStr[] = "M104 S80";

    GCodeCommand cmdOUT;

    stdErr = parse_GCodeCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(GCode_Parser_UT, M106_test)
{
    Std_Err stdErr;

    char cmdStr[] = "M106 S100";

    GCodeCommand cmdOUT;

    stdErr = parse_GCodeCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(GCode_Parser_UT, M109_test)
{
    Std_Err stdErr;

    char cmdStr[] = "M109 S80";

    GCodeCommand cmdOUT;

    stdErr = parse_GCodeCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(GCode_Parser_UT, M140_test)
{
    Std_Err stdErr;

    char cmdStr[] = "M140 S80";

    GCodeCommand cmdOUT;

    stdErr = parse_GCodeCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

TEST_F(GCode_Parser_UT, M190_test)
{
    Std_Err stdErr;

    char cmdStr[] = "M190 S80";

    GCodeCommand cmdOUT;

    stdErr = parse_GCodeCommand(cmdStr, &cmdOUT);

    stdErr = cmdOUT.init(&cmdOUT);
    EXPECT_EQ(stdErr, STD_OK);

    EXPECT_TRUE(cmdOUT.step == NULL);
    EXPECT_TRUE(cmdOUT.remove == NULL);
}

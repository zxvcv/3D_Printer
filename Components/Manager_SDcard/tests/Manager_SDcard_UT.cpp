#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_FIFO_void.h"
#include "Mock_GCode_Parser.h"
#include "Mock_SD.h"

using ::testing::_;
using ::testing::Return;

extern "C"
{
    #include "Manager_SDcard.h"
}

class Manager_SDcard_UT: public ::testing::Test
{
public:
    Manager_SDcard_UT()
    {
        mock_FIFO_void = new Mock_FIFO_void();
        mock_GCode_Parser = new Mock_GCode_Parser();
        mock_SD = new Mock_SD();
    }
    ~Manager_SDcard_UT()
    {
        delete mock_FIFO_void;
        delete mock_GCode_Parser;
        delete mock_SD;
    }

    virtual void SetUp()
    {
        Std_Err stdErr;

        settings = (SDCard_Settings*)malloc(sizeof(SDCard_Settings));
        file = (FIL*)malloc(sizeof(FIL));
        motors[0] = (Motor*)malloc(sizeof(Motor));

        EXPECT_CALL(*mock_FIFO_void, fifo_create(_))
            .Times(1)
            .WillOnce(Return(STD_OK));
        EXPECT_CALL(*mock_GCode_Parser, init_GCodeParser(motors))
            .Times(1);
        stdErr = init_manager_SDcard(settings, motors);
        EXPECT_EQ(stdErr, STD_OK);
    }

    virtual void TearDown()
    {
        free(settings);
        free(file);
        free(motors[0]);
    }

    SDCard_Settings* settings;
    FIL* file;
    Motor* motors[1];

    /* MOCKS */
    static Mock_FIFO_void* mock_FIFO_void;
    static Mock_GCode_Parser* mock_GCode_Parser;
    static Mock_SD* mock_SD;
};

Mock_FIFO_void* Manager_SDcard_UT::mock_FIFO_void;
Mock_GCode_Parser* Manager_SDcard_UT::mock_GCode_Parser;
Mock_SD* Manager_SDcard_UT::mock_SD;

/************************** TESTS **************************/

TEST_F(Manager_SDcard_UT, parse_command_SDcard_test)
{
    //TODO ...
}

TEST_F(Manager_SDcard_UT, execute_command_SDcard_test)
{
    //TODO ...
}

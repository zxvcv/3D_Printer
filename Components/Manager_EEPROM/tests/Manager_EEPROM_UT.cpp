#include <gtest/gtest.h>

#include "Mock_Project_Config.h"
#include "Mock_EEPROM_24AA01.h"

using ::testing::_;
using ::testing::Return;

extern "C"
{
    #include "Manager_EEPROM.h"
}

class Manager_EEPROM_UT: public ::testing::Test
{
public:
    Manager_EEPROM_UT()
    {
        mock_EEPROM_24AA01 = new Mock_EEPROM_24AA01();
    }
    ~Manager_EEPROM_UT()
    {
        delete mock_EEPROM_24AA01;
    }

    virtual void SetUp()
    {
        eeprom_settigns = (EEPROMSettings*)malloc(sizeof(EEPROMSettings));
        dataOUT = (MotorData_EEPROM*)malloc(sizeof(MotorData_EEPROM));
    }

    virtual void TearDown()
    {
        free(eeprom_settigns);
        free(dataOUT);
    }

    EEPROMSettings* eeprom_settigns;
    MotorData_EEPROM* dataOUT;

    /* MOCKS */
    static Mock_EEPROM_24AA01* mock_EEPROM_24AA01;
};

Mock_EEPROM_24AA01* Manager_EEPROM_UT::mock_EEPROM_24AA01;

/************************** TESTS **************************/

TEST_F(Manager_EEPROM_UT, get_motor_data_EEPROM_test)
{
    Std_Err stdErr;

    EXPECT_CALL(*mock_EEPROM_24AA01, EEPROM_readData(eeprom_settigns, _, _, _))
        .Times(1)
        .WillOnce(Return(STD_OK));
    stdErr = get_motor_data_EEPROM(eeprom_settigns, 0, dataOUT);
    EXPECT_EQ(stdErr, STD_OK);
}

TEST_F(Manager_EEPROM_UT, set_motor_data_EEPROM_test)
{
    Std_Err stdErr;

    EXPECT_CALL(*mock_EEPROM_24AA01, EEPROM_writeData(eeprom_settigns, _, _, _))
        .Times(1)
        .WillOnce(Return(STD_OK));
    stdErr = set_motor_data_EEPROM(eeprom_settigns, 0, dataOUT);
    EXPECT_EQ(stdErr, STD_OK);
}
